#include "CCCC/wrappers/cccc.h"
#include "CCCC/core/CCCC.hpp"

#include <iostream>
using namespace std;

namespace DKRZ {
CCCCt* CCCC_init(MPI_Fint *glob, int nprocs_kernel, char* backend_name) {
    cout << "C API, CCCC_init" << endl;
    // needed convertions
    string backend_name_cpp = backend_name;
    MPI_Comm g = MPI_Comm_f2c(*glob);
    return new CCCC(g, nprocs_kernel, backend_name_cpp);
}

void CCCC_finalize(CCCCt* cccc){
    cout << "C API, CCCC_finalize" << endl;
    delete cccc;
}

int CCCC_intercomm_create(CCCCt* cccc, int nmodel, int nprocs) {
	cout << "C API, CCCC_intercomm_create" << endl;
    return cccc->intercomm_create(nmodel, nprocs);
}
}
