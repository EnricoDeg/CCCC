#include <memory>

#include <mpi.h>

//extern "C"{
//#include "yaxt.h"
//}


namespace DKRZ {

 class Yaxt {
 public:
 	typedef std::shared_ptr<Yaxt> Ptr;
 	Yaxt(int glob);
 	~Yaxt();
 private:


 };

}
