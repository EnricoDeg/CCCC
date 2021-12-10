#include <vector>

#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {

  class CCCC {
    public:
    	CCCC(int glob, int nprocs_kernel);
    	~CCCC();
    	int intercomm_create(int nmodel) const;

    protected:
    	Yaxt::Ptr m_yaxt;

    private:
    	std::vector<int> m_nprocs;
  };

}