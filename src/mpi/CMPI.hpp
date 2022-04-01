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
        MPI_Comm intercomm_create(int nmodel, int nprocs, std::vector<int> nprocs_all);
        void get_mpi_datatype(int typeclass, int size, MPI_Datatype * tmpi);
        int get_cmd(MPI_Comm intercomm);
        void send_cmd(int cmd, MPI_Comm intercomm);
        void send(int nmodel, double *data, int count, MPI_Comm intercomm);
        void recv(int nmodel, double *data, int count, MPI_Comm intercomm);
        template <typename T>
        void exchange(T *data, int size, bool sender, MPI_Datatype Tmpi, MPI_Comm intercomm);
        int mymodel();
        MPI_Comm local_comm();

    private:
        MPI_Comm m_glob_comm, m_local_comm;
        int m_glob_rank, m_local_rank;
        int m_mymodel;
  };

}
