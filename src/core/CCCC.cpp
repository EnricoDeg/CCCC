#include <iostream>

#include "CCCC/core/CCCC.hpp"
#include "CCCC/grid/GridStruct.hpp"
#include "CCCC/grid/GridUnstruct.hpp"
#include "CCCC/grid/GridGauss.hpp"

namespace DKRZ {

    Grid_Backend choose_backend(std::string backend_name) {
        if (backend_name == "structured") {
          return CCCC_STRUCTURED;
        } else if (backend_name == "unstructured") {
          return CCCC_UNSTRUCTURED;
        } else if (backend_name == "gaussian") {
          return CCCC_GAUSSIAN;
        } else {
          return CCCC_NONE;
        }
    }

    Grid::Ptr create_backend(Grid_Backend backend, Yaxt::Ptr yaxt) {
        if (backend == CCCC_STRUCTURED) {
          return Grid::Ptr(new GridStruct(yaxt));
        } else if (backend == CCCC_UNSTRUCTURED) {
          return Grid::Ptr(new GridUnstruct(yaxt));
        } else if (backend == CCCC_GAUSSIAN) {
          return Grid::Ptr(new GridGauss(yaxt));
        }
    }
    
    // Constructor
    CCCC::CCCC(MPI_Comm glob, int nprocs_kernel, std::string backend_name) {
        // initialize YAXT library
        int world_rank;
        m_mpi.reset(new MPI(glob));
        m_yaxt.reset(new Yaxt(glob));
        m_nprocs.push_back(nprocs_kernel);
        MPI_Comm_rank(glob, &world_rank);
        if (world_rank < nprocs_kernel) {
            m_kernel_role = false;
        } else {
            m_kernel_role = true;
        }
        // create backend to grid object
        Grid_Backend backend = choose_backend(backend_name);
        if (backend == CCCC_NONE)
            handle_error("selected grid type is not implemented");
        m_grid = create_backend(backend, m_yaxt);
    }

    // Destructor
    CCCC::~CCCC() {
        // reset grid object pointer
        m_grid.reset();
        // finalize YAXT library
        m_yaxt.reset();
    }

    // Create intercommunicator for each model
    void CCCC::intercomm_create(int nmodel, int nprocs) {
        int world_rank;
        if ( nmodel != m_nprocs.size() )
            handle_error("kernels ID should be defined sequentially (i.e. 1,2,3,4,...) in intrercomm_create() function");
        if ( nmodel <= 0)
            handle_error("kernels ID must be positive in intrercomm_create() function");
        m_nprocs.push_back(nprocs);
        m_mpi->intercomm_create(nmodel, nprocs, m_nprocs);
        m_yaxt->set_mr_intercomm(m_mpi->intercomm());
    }

    // set grid info
    void CCCC::grid_subdomain_start(int i, int j) {
        m_grid->set_subdomain_start(i, j);
    }

    void CCCC::grid_subdomain_end(int i, int j) {
        m_grid->set_subdomain_end(i, j);
    }

    void CCCC::grid_subdomain_ext(int i, int j) {
        m_grid->set_subdomain_ext(i, j);
    }

    void CCCC::grid_subdomain_off(int i, int j) {
        m_grid->set_subdomain_off(i, j);
    }

    void CCCC::grid_domain_ext(int i, int j) {
        m_grid->set_domain_ext(i, j);
    }

    // create YAXT redist
    void CCCC::set_redist(int nmodel, int nlv) {
        m_redist[nlv] = m_grid->set_yaxt_redist(nmodel, nlv, m_kernel_role);
    }

    // Add fields, variables and commands to each model
    void CCCC::add_field(double *data, int nlv, int nmodel, int id, bool m2k) {
        FieldType f;
        f.data = data;
        if (m2k == true) {
            f.redist = &(m_redist[nlv].redist_m2k);
        } else {
            f.redist = &(m_redist[nlv].redist_k2m);
        }
        f.exchange_id = id;
        f.m2k = m2k;
        m_fields[nmodel].push_back(f);
    }

    void CCCC::add_variable(double *data, int count, int nmodel, int id, bool m2k) {
        if (count <= 0)
            handle_error("count must be a positive integer in add_variable() function");
        VariableType f;
        f.data = data;
        f.count = count;
        f.exchange_id = id;
        f.m2k = m2k;
        m_variables[nmodel].push_back(f);
    }

    void CCCC::add_command(void (*Func_ptr) (), int nmodel, int cmd_id) {
        if (cmd_id < 0)
            handle_error("command ID can not be negative in add_command() function");
        CmdType c;
        c.cmd = Func_ptr;
        c.cmd_id = cmd_id;
        m_cmds[nmodel].push_back(c);
    }

    // Start and stop concurrent execution
    void CCCC::start_concurrent(int nmodel) {
        if (m_kernel_role) {
          remote_loop(nmodel);
        }
    }

    void CCCC::stop_concurrent(int nmodel) {
        if (!m_kernel_role) {
          m_mpi->send_cmd(nmodel, CCCC_CMD_EXIT);
        }
    }

    void CCCC::remote_loop(int nmodel) {
        if (nmodel != m_mpi->mymodel())
            return;
        int i_cmd = m_mpi->get_cmd(nmodel);
        std::cout << "Remote loop starts" << std::endl;
        int id;
        while (i_cmd != CCCC_CMD_EXIT) {
            switch(i_cmd) {
                case(CCCC_K2M):
                    id = m_mpi->get_cmd(nmodel);
                    exchange_k2m_impl(nmodel, id);
                    break;
                case(CCCC_M2K):
                    id = m_mpi->get_cmd(nmodel);
                    exchange_m2k_impl(nmodel,id);
                    break;
                default:
                    for (auto stream_cmds : m_cmds[nmodel]) {
                        if (stream_cmds.cmd_id == i_cmd) {
                            stream_cmds.cmd(); // execute command with received ID
                        }  
                    }
                    break;
            }
            i_cmd = m_mpi->get_cmd(nmodel);
        }
    }

    // Exchange fields and variables between model and kernels
    void CCCC::exchange_k2m(int nmodel, int id) {
        if (!m_kernel_role) {
            m_mpi->send_cmd(nmodel, CCCC_K2M);
            m_mpi->send_cmd(nmodel, id);
            exchange_k2m_impl(nmodel, id);
        } else {
            handle_error("kernels procs should not call exchange_k2m() function");
        }
    }

    void CCCC::exchange_k2m_impl(int nmodel, int id) {
        if (m_kernel_role) {
            send_data(nmodel, id, false);
        } else {
            recv_data(nmodel, id, false);
        }
        exchange_field(nmodel, id, false);
    }

    void CCCC::exchange_m2k(int nmodel, int id) {
        if (!m_kernel_role) {
            m_mpi->send_cmd(nmodel, CCCC_M2K);
            m_mpi->send_cmd(nmodel, id);
            exchange_m2k_impl(nmodel, id);
        } else {
            handle_error("kernels procs should not call exchange_m2k() function");
        }
    }

    void CCCC::exchange_m2k_impl(int nmodel, int id) {
        if (m_kernel_role) {
            recv_data(nmodel, id, true);
        } else {
            send_data(nmodel, id, true);
        }
        exchange_field(nmodel, id, true);
    }

    void CCCC::send_data(int nmodel, int id, bool cond) {
        for (auto variable : m_variables[nmodel]) {
            if (variable.m2k == cond && variable.exchange_id == id) {
                m_mpi->send(nmodel, variable.data, variable.count);
            }
        }
    }

    void CCCC::recv_data(int nmodel, int id, bool cond) {
        for (auto variable : m_variables[nmodel]) {
            if (variable.m2k == cond && variable.exchange_id == id) {
                m_mpi->recv(nmodel, variable.data, variable.count);
            }
        }
    }

    void CCCC::exchange_field(int nmodel, int id, bool cond) {
        for (auto field : m_fields[nmodel]) {
            if (field.m2k == cond && field.exchange_id == id) {
                m_yaxt->exchange(*(field.redist), field.data);
            }
        }
    }

    // Execute commands labeled with an ID for a model
    void CCCC::execute(int nmodel, int cmd_id) {
        if (!m_kernel_role) {
            m_mpi->send_cmd(nmodel, cmd_id);
        } else {
            handle_error("kernels procs should not call execute() function");
        }
    }

    // Return useful info
    bool CCCC::has_kernel_role() {
        return m_kernel_role;
    }

}
