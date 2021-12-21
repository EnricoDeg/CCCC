#include <mpi.h>
#include "CCCC/core/CCCC.hpp"

using namespace DKRZ;

int main(int argc, char **argv)
{
  MPI_Init(NULL, NULL);

  std::string s = "structured";
  std::shared_ptr<CCCC> ccm;
  ccm.reset(new CCCC(MPI_COMM_WORLD, 2, s));

  int i = ccm->intercomm_create(1, 1);
  i = ccm->intercomm_create(2, 1);

  ccm.reset();

  MPI_Finalize();
}
