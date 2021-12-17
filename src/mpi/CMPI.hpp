#include <vector>
#include <memory>

#include <mpi.h>

namespace DKRZ {

  class MPI {
    public:
        typedef std::shared_ptr<MPI> Ptr;
    	MPI(MPI_Comm glob);
    	~MPI();
        void intercomm_create(int nmodel, int nprocs, std::vector<int> nprocs_all);
        int get_cmd();
        void send_cmd(int nmodel, int cmd);
        MPI_Comm intercomm(int i);

    private:
        MPI_Comm m_glob_comm, m_local_comm;
        int m_glob_rank, m_local_rank;
        std::vector<MPI_Comm> m_intercomm;
  };

}
