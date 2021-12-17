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

    CCCC::~CCCC() {
        // reset grid object pointer
        m_grid.reset();
        // finalize YAXT library
        m_yaxt.reset();
    }

    int CCCC::intercomm_create(int nmodel, int nprocs) {
        int world_rank;
        if ( nmodel != m_nprocs.size() ) return 127;
        m_nprocs.push_back(nprocs);
        m_mpi->intercomm_create(nmodel, nprocs, m_nprocs);
        return 0;
    }

    void CCCC::set_redist(int nlv) {

    }

    void CCCC::add_field(double *data, int nlv, int nmodel, bool m2k) {
        FieldType f;
        f.data = data;
        f.redist = &m_redist[nlv];
        f.m2k = m2k;
        m_fields[nmodel].push_back(f);
    }

    void CCCC::add_command(void (*Func_ptr) (), int nmodel, int cmd_id) {
        CmdType c;
        c.cmd = Func_ptr;
        c.cmd_id = cmd_id;
        m_cmds[nmodel].push_back(c);
    }

    void CCCC::start_concurrent(int nmodel) {
        if (m_kernel_role) {
          remote_loop(nmodel);
        }
    }

    void CCCC::remote_loop(int nmodel) {
        int i_cmd = m_mpi->get_cmd();
        std::cout << "Remote loop starts" << std::endl;
        while (i_cmd != CCCC_CMD_EXIT) {
            for (auto stream_cmds : m_cmds[nmodel]) {
                if (stream_cmds.cmd_id == i_cmd) {
                    stream_cmds.cmd(); // execute command with received ID
                }  
            }
            i_cmd = m_mpi->get_cmd();
        }
    }

    void CCCC::stop_concurrent(int nmodel) {
        if (!m_kernel_role) {
          m_mpi->send_cmd(nmodel, CCCC_CMD_EXIT);
        }
    }

    void CCCC::intercomm_test() {

        if (!m_kernel_role) {
            for (int i = 1; i < m_nprocs.size(); i++ ) {
                MPI_Send(&i, 1, MPI_INT, 0, 0, m_mpi->intercomm(i-1));
            }
        } else {
            MPI_Status stat;
            int i, world_rank;
            MPI_Comm_rank(m_yaxt->glob(), &world_rank);
            MPI_Recv(&i, 1, MPI_INT, 0, 0, m_mpi->intercomm(0), &stat);
            std::cout << world_rank << ": i = " << i << std::endl;
        }

    }

    void CCCC::execute(int nmodel, int cmd_id) {
        if (!m_kernel_role) {
            m_mpi->send_cmd(nmodel, cmd_id);
        }
    }

    bool CCCC::has_kernel_role() {
        return m_kernel_role;
    }
    
}
