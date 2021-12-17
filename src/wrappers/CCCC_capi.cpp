#include "CCCC/wrappers/cccc.h"
#include "CCCC/core/CCCC.hpp"

#include <iostream>

namespace DKRZ {
    CCCCt* CCCC_init(MPI_Fint *glob, int nprocs_kernel, char* backend_name) {
        std::cout << "C API, CCCC_init" << std::endl;
        // needed convertions
        std::string backend_name_cpp = backend_name;
        MPI_Comm g = MPI_Comm_f2c(*glob);
        return new CCCC(g, nprocs_kernel, backend_name_cpp);
    }

    void CCCC_finalize(CCCCt* cccc){
        std::cout << "C API, CCCC_finalize" << std::endl;
        delete cccc;
    }

    int CCCC_intercomm_create(CCCCt* cccc, int nmodel, int nprocs) {
        std::cout << "C API, CCCC_intercomm_create" << std::endl;
        return cccc->intercomm_create(nmodel, nprocs);
    }

    void CCCC_start_concurrent(CCCCt* cccc, int nmodel) {
        std::cout << "C API, CCCC_start_concurrent" << std::endl;
        return cccc->start_concurrent(nmodel);
    }

    void CCCC_stop_concurrent(CCCCt* cccc, int nmodel) {
        std::cout << "C API, CCCC_stop_concurrent" << std::endl;
        return cccc->stop_concurrent(nmodel);
    }

    bool CCCC_has_kernel_role(CCCCt* cccc) {
        return cccc->has_kernel_role();
    }

}
