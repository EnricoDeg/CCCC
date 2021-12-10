#include <vector>

#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {

  class CCCC {
    public:
    	CCCC(MPI_Comm glob, int nprocs_kernel);
    	~CCCC();
    	int intercomm_create(int nmodel, int nprocs);

    protected:
    	Yaxt::Ptr m_yaxt;

    private:
    	std::vector<int> m_nprocs;
  };

}
