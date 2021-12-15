#include <vector>

#include "CCCC/yaxt/Yaxt.hpp"
#include "CCCC/grid/Grid.hpp"

namespace DKRZ {

  class CCCC {
    public:
    	CCCC(MPI_Comm glob, int nprocs_kernel, std::string backend_name);
    	~CCCC();
    	int intercomm_create(int nmodel, int nprocs);
        void intercomm_test();

    protected:
    	Yaxt::Ptr m_yaxt;
        Grid::Ptr m_grid;

    private:
        bool m_kernel_role;
        MPI_Comm m_local_comm;
    	std::vector<int> m_nprocs;
        std::vector<MPI_Comm> m_intercomm;
  };

}
