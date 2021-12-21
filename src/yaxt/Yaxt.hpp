#include <memory>
#include <map>

#include <mpi.h>

extern "C"{
#include "yaxt.h"
}


namespace DKRZ {

    struct YaxtRedist {
        Xt_redist redist_m2k;
        Xt_redist redist_k2m;
    };

    class Yaxt {
        public:
        typedef std::shared_ptr<Yaxt> Ptr;
        Yaxt(MPI_Comm glob);
        ~Yaxt();
        void exchange(Xt_redist redist, double *data);
        MPI_Comm glob();

        private:
        MPI_Comm m_glob;
        Xt_idxlist m_empty_redist;
    };

}
