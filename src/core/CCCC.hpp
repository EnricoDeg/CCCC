#include <vector>

#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {

  class CCCC {
    public:
    	CCCC(MPI_Comm glob, int nprocs_kernel);
    	~CCCC();
    	int intercomm_create(int nmodel, int nprocs);
        void intercomm_test();

    protected:
    	Yaxt::Ptr m_yaxt;

    private:
        bool m_kernel_role;
        MPI_Comm m_local_comm;
    	std::vector<int> m_nprocs;
        std::vector<MPI_Comm> m_intercomm;
  };

}
