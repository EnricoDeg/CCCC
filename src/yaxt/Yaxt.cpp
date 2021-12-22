#include "CCCC/yaxt/Yaxt.hpp"

namespace DKRZ {
    
    Yaxt::Yaxt(MPI_Comm glob) {
        xt_initialize(glob);
        m_glob = glob;
        m_empty_idxlist = xt_idxempty_new();
        m_empty_off = 0;
    }
    
    Yaxt::~Yaxt() {
        xt_finalize();
    }
    
    void Yaxt::exchange(Xt_redist redist, double *data) {
        xt_redist_s_exchange1(redist, data, data);
    }

    void Yaxt::set_mr_intercomm(std::map<int,MPI_Comm> intercomm) {
        for ( auto it = intercomm.begin(); it != intercomm.end(); ++it  )
        {
            MPI_Comm com;
            MPI_Comm_dup(it->second, &com);
            m_mr_intercomm[it->first] = com;
            xt_mpi_comm_mark_exclusive(m_mr_intercomm[it->first]);
        }
    }
    
    Xt_idxlist Yaxt::get_idxlist(Xt_int *idx, int size) {
        return xt_idxvec_new(idx, size);
    }
    
    Xt_xmap Yaxt::get_map(Xt_idxlist idxlist, int nmodel, bool sender) {
        if (sender) {
            return xt_xmap_dist_dir_new(idxlist, m_empty_idxlist, m_mr_intercomm[nmodel]);
        } else {
            return xt_xmap_dist_dir_new(m_empty_idxlist, idxlist, m_mr_intercomm[nmodel]);
        }
    }

    Xt_redist Yaxt::get_redist_off(Xt_xmap map, int *idx_off, bool sender) {
        if (sender) {
            return xt_redist_p2p_off_new(map, idx_off, &m_empty_off, MPI_DOUBLE);
        } else {
            return xt_redist_p2p_off_new(map, &m_empty_off, idx_off, MPI_DOUBLE);
        }
    }
    
    MPI_Comm Yaxt::glob() {
        return m_glob;
    }
    
}
