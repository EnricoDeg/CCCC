#include <vector>
#include <map>
#include <memory>

#include <mpi.h>

namespace DKRZ {

  //! interface to MPI library
  /*!
    
  */
  class MPI {
    public:
        typedef std::shared_ptr<MPI> Ptr;
    	MPI(MPI_Comm glob);
    	~MPI();
        void intercomm_create(int nmodel, int nprocs, std::vector<int> nprocs_all);
        int get_cmd(int nmodel);
        void send_cmd(int nmodel, int cmd);
        void send(int nmodel, double *data, int count);
        void recv(int nmodel, double *data, int count);
        void exchange(double *data, int size, bool sender, int nmodel);
        MPI_Comm intercomm(int i);
        std::map<int,MPI_Comm> intercomm();
        int mymodel();
        MPI_Comm local_comm();

    private:
        MPI_Comm m_glob_comm, m_local_comm;
        int m_glob_rank, m_local_rank;
        std::map<int,MPI_Comm> m_intercomm;
        int m_mymodel;
  };

}
