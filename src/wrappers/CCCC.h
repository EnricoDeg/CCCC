#include <mpi.h>

#ifdef __cplusplus // Are we compiling this with a C++ compiler ?
extern "C" {
    class CCCC;
    typedef CCCC CCCCt;
#else
    // From the C side, we use an opaque pointer.
    typedef struct CCCCt CCCCt;
#endif

// Constructor
CCCCt* CCCC_init(MPI_Comm glob, int nprocs_kernel);

// Destructor
void CCCC_finalize(CCCCt* cccc);

// The const qualificators maps from the member function to pointers to the
// class instances.
int CCCC_intercomm_create(const CCCCt* cccc, int nmodel, int nprocs);

#ifdef __cplusplus
}
#endif
