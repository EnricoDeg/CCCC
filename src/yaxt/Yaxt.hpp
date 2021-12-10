#include <memory>

#include <mpi.h>

extern "C"{
#include "yaxt.h"
}


namespace DKRZ {

 class Yaxt {
 public:
 	typedef std::shared_ptr<Yaxt> Ptr;
 	Yaxt(MPI_Comm glob);
 	~Yaxt();
        MPI_Comm glob();
 private:
        MPI_Comm m_glob;

 };

}
