#include <mpi.h>

namespace DKRZ {
#ifdef __cplusplus // Are we compiling this with a C++ compiler ?
extern "C" {
    class CCCC;
    typedef CCCC CCCCt;
#else
    // From the C side, we use an opaque pointer.
    typedef struct CCCCt CCCCt;
#endif

// Constructor
CCCCt* CCCC_init(MPI_Fint *glob, int nprocs_kernel, char* backend_name);

// Destructor
void CCCC_finalize(CCCCt* cccc);

// Create intercommunicator for a stream (kernel)
int CCCC_intercomm_create(CCCCt* cccc, int nmodel, int nprocs);

// Start concurrent execution (infinite loop for kernel procs)
void CCCC_start_concurrent(CCCCt* cccc, int nmodel);

// Stop concurrent execution
void CCCC_stop_concurrent(CCCCt* cccc, int nmodel);

bool CCCC_has_kernel_role(CCCCt* cccc);

#ifdef __cplusplus
}
#endif
}
