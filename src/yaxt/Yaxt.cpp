#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {

Yaxt::Yaxt(MPI_Comm glob) {
	xt_initialize(glob);
}

Yaxt::~Yaxt() {
	xt_finalize();
}

}
