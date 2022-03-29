#include <mpi.h>
#include "CCCC/core/CCCC.hpp"

using namespace DKRZ;

int main(int argc, char **argv)
{
  MPI_Init(NULL, NULL);

  std::string s = "structured";
  std::shared_ptr<CCCC> ccm;
  ccm.reset(new CCCC(MPI_COMM_WORLD, 1, s));

  ccm->intercomm_create(1, 1);

  int src_array_size[2] = {10,10};
  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  double * src_array = (double *)malloc((size_t)src_array_size[0]
                              * (size_t)src_array_size[1] * sizeof(*src_array));

  for (int i = 0; i < src_array_size[0] * src_array_size[1]; ++i)
    src_array[i] = 1.0 - (double)rank;

//  ccm->grid_subdomain_start(1, 1);
//  ccm->grid_subdomain_end(10, 10);
  ccm->grid_subdomain_ext(10, 10);
//  ccm->grid_subdomain_off(0, 0);
//  ccm->grid_domain_ext(10, 10);

//  ccm->set_redist(1, 1);

  ccm->add_field(src_array, 1, 1, 1, true);

  ccm->start_concurrent(1);
  if (!ccm->has_kernel_role()) {
    ccm->exchange_m2k(1, 1);
  }
  ccm->stop_concurrent(1);

  ccm.reset();

  MPI_Finalize();
}
