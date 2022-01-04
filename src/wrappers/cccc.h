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

// get local comm of a component
MPI_Fint CCCC_local_comm(CCCCt* cccc);

// Destructor
void CCCC_finalize(CCCCt* cccc);

// Create intercommunicator for a stream (kernel)
void CCCC_intercomm_create(CCCCt* cccc, int nmodel, int nprocs);

// Start concurrent execution (infinite loop for kernel procs)
void CCCC_start_concurrent(CCCCt* cccc, int nmodel);

// Stop concurrent execution
void CCCC_stop_concurrent(CCCCt* cccc, int nmodel);

// add command to a stream
void CCCC_add_command(CCCCt* cccc, void (*Func_ptr) (), int nmodel, int cmd_id);

// Execute a command on a stream
void CCCC_execute(CCCCt* cccc, int nmodel, int cmd_id);

// Return if procs has kernel role
bool CCCC_has_kernel_role(CCCCt* cccc);

// add field to a stream
void CCCC_add_field(CCCCt* cccc, double *data, int nlv, int nmodel, int id, int m2k);

// add variable to a stream
void CCCC_add_variable(CCCCt* cccc, double *data, int count, int nmodel, int id, int m2k);

// send fields and variables to model
void CCCC_exchange_k2m(CCCCt* cccc, int nmodel, int id);

// send fields and variables to a kernel
void CCCC_exchange_m2k(CCCCt* cccc, int nmodel, int id);

// grid functions
void CCCC_grid_subdomain_start(CCCCt* cccc, int i, int j);
void CCCC_grid_subdomain_end(CCCCt* cccc, int i, int j);
void CCCC_grid_subdomain_ext(CCCCt* cccc, int i, int j);
void CCCC_grid_subdomain_off(CCCCt* cccc, int i, int j);
void CCCC_grid_domain_ext(CCCCt* cccc, int i, int j);

// set YAXT redistribution
void CCCC_set_redist(CCCCt* cccc, int nmodel, int nlv);

#ifdef __cplusplus
}
#endif
}
