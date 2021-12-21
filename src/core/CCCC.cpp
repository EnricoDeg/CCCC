#include <iostream>

#include "CCCC/core/CCCC.hpp"
#include "CCCC/grid/GridStruct.hpp"

namespace DKRZ {

    Grid_Backend choose_backend(std::string backend_name) {
        if (backend_name == "structured") {
          return CCCC_STRUCTURED;
        }
    }

    Grid::Ptr create_backend(Grid_Backend backend) {
        if (backend == CCCC_STRUCTURED) {
          return Grid::Ptr(new GridStruct());
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
        m_grid = create_backend(backend);
    }

    // Destructor
    CCCC::~CCCC() {
        // reset grid object pointer
        m_grid.reset();
        // finalize YAXT library
        m_yaxt.reset();
    }

    // Create intercommunicator for each model
    int CCCC::intercomm_create(int nmodel, int nprocs) {
        int world_rank;
        if ( nmodel != m_nprocs.size() ) return 127;
        m_nprocs.push_back(nprocs);
        m_mpi->intercomm_create(nmodel, nprocs, m_nprocs);
        return 0;
    }

    void CCCC::set_redist(int nlv) {

    }

    // Add fields, variables and commands to each model
    void CCCC::add_field(double *data, int nlv, int nmodel, bool m2k) {
        FieldType f;
        f.data = data;
        if (m2k == true) {
            f.redist = &(m_redist[nlv].redist_m2k);
        } else {
            f.redist = &(m_redist[nlv].redist_k2m);
        }
        f.m2k = m2k;
        m_fields[nmodel].push_back(f);
    }

    void CCCC::add_variable(double *data, int count, int nmodel, bool m2k) {
        VariableType f;
        f.data = data;
        f.count = count;
        f.m2k = m2k;
        m_variables[nmodel].push_back(f);
    }

    void CCCC::add_command(void (*Func_ptr) (), int nmodel, int cmd_id) {
        if (cmd_id < 0)
            handle_error("command ID can not be negative");
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
        while (i_cmd != CCCC_CMD_EXIT) {
            switch(i_cmd) {
                case(CCCC_K2M):
                    exchange_k2m_impl(nmodel);
                    break;
                case(CCCC_M2K):
                    exchange_m2k_impl(nmodel);
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
    void CCCC::exchange_k2m(int nmodel) {
        if (!m_kernel_role) {
          m_mpi->send_cmd(nmodel, CCCC_K2M);
          exchange_k2m_impl(nmodel);
        }
    }

    void CCCC::exchange_k2m_impl(int nmodel) {
        if (m_kernel_role) {
            send_data(nmodel, false);
        } else {
            recv_data(nmodel, false);
        }
        exchange_field(nmodel, false);
    }

    void CCCC::exchange_m2k(int nmodel) {
        if (!m_kernel_role) {
          m_mpi->send_cmd(nmodel, CCCC_M2K);
          exchange_m2k_impl(nmodel);
        }
    }

    void CCCC::exchange_m2k_impl(int nmodel) {
        if (m_kernel_role) {
            recv_data(nmodel, true);
        } else {
            send_data(nmodel, true);
        }
        exchange_field(nmodel, true);
    }

    void CCCC::send_data(int nmodel, bool cond) {
        for (auto variable : m_variables[nmodel]) {
            if (variable.m2k == cond) {
                m_mpi->send(nmodel, variable.data, variable.count);
            }
        }
    }

    void CCCC::recv_data(int nmodel, bool cond) {
        for (auto variable : m_variables[nmodel]) {
            if (variable.m2k == cond) {
                m_mpi->recv(nmodel, variable.data, variable.count);
            }
        }
    }

    void CCCC::exchange_field(int nmodel, bool cond) {
        for (auto field : m_fields[nmodel]) {
            if (field.m2k == cond) {
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

    // Error handler
    void CCCC::handle_error(std::string msg) {
        std::cerr << "CCCC Error: " << msg << std::endl;
        std::exit (EXIT_FAILURE);
    }
    
}
