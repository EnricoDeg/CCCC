#include <mpi.h>
#include "CCCC/core/CCCC.hpp"

using namespace DKRZ;

int main(int argc, char **argv)
{
  MPI_Init(NULL, NULL);

  std::shared_ptr<CCCC> ccm;
  ccm.reset(new CCCC(MPI_COMM_WORLD, 2));

  int i = ccm->intercomm_create(1, 1);
  i = ccm->intercomm_create(2, 1);
  ccm->intercomm_test();

  ccm.reset();

  MPI_Finalize();
}
