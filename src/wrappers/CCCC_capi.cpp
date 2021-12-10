#include "CCCC/wrappers/cccc.h"
#include "CCCC/core/CCCC.hpp"

#include <iostream>
using namespace std;

namespace DKRZ {
CCCCt* CCCC_init(MPI_Comm glob, int nprocs_kernel) {
    cout << "C API, CCCC_init" << endl;
    return new CCCC(glob, nprocs_kernel);
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
