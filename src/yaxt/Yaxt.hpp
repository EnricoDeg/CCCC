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
    
    struct YaxtMap {
        Xt_xmap map_m2k;
        Xt_xmap map_k2m;
    };
    
    class Yaxt {
        public:
            typedef std::shared_ptr<Yaxt> Ptr;
            Yaxt(MPI_Comm glob);
            ~Yaxt();
            void set_mr_intercomm(std::map<int,MPI_Comm> intercomm);
            void exchange(Xt_redist redist, double *data);
            Xt_idxlist get_idxlist(int *idx, int size);
            Xt_xmap get_map(Xt_idxlist idxlist, int nmodel, bool sender);
            Xt_redist get_redist_off(Xt_xmap map, int *idx_off, bool sender);
            MPI_Comm glob();
        
        private:
            MPI_Comm m_glob;
            Xt_idxlist m_empty_idxlist;
            std::map<int,MPI_Comm> m_mr_intercomm;
            int m_empty_off;
    };
    
}
