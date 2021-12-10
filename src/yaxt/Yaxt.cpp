#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {

Yaxt::Yaxt(MPI_Comm glob) {
    xt_initialize(glob);
    m_glob = glob;
}

Yaxt::~Yaxt() {
    xt_finalize();
}

MPI_Comm Yaxt::glob() {
    return m_glob;
}

}
