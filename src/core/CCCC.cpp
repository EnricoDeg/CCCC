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
        MPI_Comm intercomm = m_mpi->intercomm_create(nmodel, nprocs, m_nprocs);
        //m_yaxt->set_mr_intercomm();
        // create component obj
        m_components.emplace(nmodel, new Component(intercomm, m_yaxt, m_mpi));
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

    // [[[INTERFACE]]] Add fields, variables and commands to each model
    void CCCC::add_field(double *data, int nlv, int nmodel, int id, bool m2k) {
        if (m_kernel_role && nmodel != m_mpi->mymodel())
            return;
        Xt_redist *rds;
        if (m2k == true) {
            rds = &(m_redist[nlv].redist_m2k);
        } else {
            rds = &(m_redist[nlv].redist_k2m);
        }
        m_components[nmodel]->add_field_impl(data, rds, m_grid->get_size() * nlv, 
                                            id, m2k);
    }

    // [[[INTERFACE]]]
    template <typename T>
    void CCCC::add_variable(T *data, int count, int nmodel, int id, bool m2k) {
        if (m_kernel_role && nmodel != m_mpi->mymodel())
            return;
        if (count <= 0)
            handle_error("count must be a positive integer in add_variable() function");
        m_components[nmodel]->add_variable_impl<T>(data, count, id, m2k);
    }
    template void CCCC::add_variable<double>(double *, int, int, int, bool);
    template void CCCC::add_variable<float>(float *, int, int, int, bool);
    template void CCCC::add_variable<int>(int *, int, int, int, bool);

    // [[[INTERFACE]]]
    void CCCC::add_command(void (*Func_ptr) (), int nmodel, int cmd_id) {
        if (m_kernel_role && nmodel != m_mpi->mymodel())
            return;
        if (cmd_id < 0)
            handle_error("command ID can not be negative in add_command() function");
        m_components[nmodel]->add_command_impl(Func_ptr, cmd_id);
    }

    // [[[INTERFACE]]] kernel procs start concurrent execution going into an
    // infinite loop and waiting for instructions
    void CCCC::start_concurrent(int nmodel) {
        // Any procs can call this interface but model procs return and also
        // procs of different targeted kernel
        if (m_kernel_role) {
            if (nmodel != m_mpi->mymodel())
                return;
            m_components[nmodel]->remote_loop_impl();
        }
    }

    // [[[INTERFACE]]] model send command to kernel to stop (exit infinite loop)
    void CCCC::stop_concurrent(int nmodel) {
        // Any procs can call this interface but only model procs do sth
        if (!m_kernel_role)
            m_components[nmodel]->send_cmd(CCCC_CMD_EXIT);
    }

    // [[[INTERFACE]]] Exchange fields and variables between model and kernels
    void CCCC::exchange_k2m(int nmodel, int id) {
        // only the main model can trigger and exchange in both directions
        if (!m_kernel_role) {
            m_components[nmodel]->send_cmd(CCCC_K2M);
            m_components[nmodel]->send_cmd(id);
            full_exchange(nmodel, id, false);
        } else {
            handle_error("kernels procs should not call exchange_k2m() function");
        }
    }

    // [[[INTERFACE]]]
    void CCCC::exchange_m2k(int nmodel, int id) {
        if (!m_kernel_role) {
            m_components[nmodel]->send_cmd(CCCC_M2K);
            m_components[nmodel]->send_cmd(id);
            full_exchange(nmodel, id, true);
        } else {
            handle_error("kernels procs should not call exchange_m2k() function");
        }
    }

    // full exchange (variables and fields) 
    void CCCC::full_exchange(int nmodel, int id, bool cond) {
        if (!m_kernel_role) {
            bool sender;

            if (cond) { // m2k
                if (m_kernel_role) {
                    sender = false; // kernel recv data
                } else {
                    sender = true; // model send data
                }
            } else { //k2m
                if (m_kernel_role) {
                    sender = true; // kernel send data
                } else {
                    sender = false; // model recv data
                }
            }

            m_components[nmodel]->exchange_variable_mpi_impl(id, cond, sender);
            m_components[nmodel]->exchange_field_mpi_impl(id, cond, sender);
            //m_components[nmodel]->exchange_field_yaxt_impl(id, cond);
        } else {
            handle_error("kernels procs should not call full_exchange() function");
        }
    }

    // [[[INTERFACE]]] Execute commands labeled with an ID for a model
    void CCCC::execute(int nmodel, int cmd_id) {
        // only the main model can send commands to single kernels
        if (!m_kernel_role) {
            m_components[nmodel]->send_cmd(cmd_id);
        } else {
            handle_error("kernels procs should not call execute() function");
        }
    }

    // Return useful info
    bool CCCC::has_kernel_role() {
        return m_kernel_role;
    }

    MPI_Comm CCCC::local_comm() {
        return m_mpi->local_comm();
    }

}
