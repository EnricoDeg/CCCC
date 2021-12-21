#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {

    Yaxt::Yaxt(MPI_Comm glob) {
        xt_initialize(glob);
        m_glob = glob;
        m_empty_redist = xt_idxempty_new();
    }

    Yaxt::~Yaxt() {
        xt_finalize();
    }

    void Yaxt::exchange(Xt_redist redist, double *data) {
        xt_redist_s_exchange1(redist, data, data);
    }

    MPI_Comm Yaxt::glob() {
        return m_glob;
    }

}
