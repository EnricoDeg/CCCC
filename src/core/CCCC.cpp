
#include "CCCC/core/CCCC.hpp"

namespace DKRZ {
    
    CCCC::CCCC(MPI_Comm glob, int nprocs_kernel) {
    	// initialize YAXT library
    	m_yaxt.reset(new Yaxt(glob));
    	m_nprocs.push_back(nprocs_kernel);
    }

    CCCC::~CCCC() {
    	// finalize YAXT library
    	m_yaxt.reset();
    }

    int CCCC::intercomm_create(int nmodel) const {
        return 127;
    }

}
