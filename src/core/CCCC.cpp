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
    	m_yaxt.reset(new Yaxt(glob));
    	m_nprocs.push_back(nprocs_kernel);
        MPI_Comm_rank(glob, &world_rank);
        if (world_rank < nprocs_kernel) {
            m_kernel_role = false;
        } else {
            m_kernel_role = true;
        }
        m_local_comm = MPI_COMM_NULL;
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
        MPI_Comm_rank(m_yaxt->glob(), &world_rank);

        int lb = 0;
        for (int i = 0; i < nmodel; i++)
            lb += m_nprocs[i];

        int ikey, icolor, ilocal_root, iremote_root;
        ikey = world_rank;
        if (world_rank < m_nprocs[0]) {
            // Kernel case
            icolor = 1;
            ilocal_root = 0;
            iremote_root = lb;
        } else if (world_rank >= lb  && world_rank< lb+nprocs) {
            // Model case
            icolor = 2;
            ilocal_root = 0;
            iremote_root = 0;
        } else {
            // Other models (to ignore in this call)
            icolor = 3;
            ilocal_root = 0;
            iremote_root = -1;
        }
        MPI_Comm local_comm, intercomm;
        MPI_Comm_split(m_yaxt->glob(), icolor, ikey, &local_comm);
        if (iremote_root != -1) {
            MPI_Intercomm_create(local_comm, ilocal_root, 
                                 m_yaxt->glob(), iremote_root, 
                                 0, &intercomm);
            m_intercomm.push_back(intercomm);
            if (m_local_comm == MPI_COMM_NULL) m_local_comm = local_comm;
        }
        return 0;
    }

    void CCCC:: intercomm_test() {
        if (!m_kernel_role) {
            for (int i = 1; i < m_nprocs.size(); i++ ) {
                MPI_Send(&i, 1, MPI_INT, 0, 0, m_intercomm[i-1]);
            }
        } else {
            MPI_Status stat;
            int i, world_rank;
            MPI_Comm_rank(m_yaxt->glob(), &world_rank);
            MPI_Recv(&i, 1, MPI_INT, 0, 0, m_intercomm[0], &stat);
            std::cout << world_rank << ": i = " << i << std::endl;
        }

    }
    
}
