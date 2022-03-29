#include <iostream>
#include <vector>

#include "GridStruct.hpp"

namespace DKRZ {
    
    GridStruct::GridStruct(Yaxt::Ptr yaxtp) : Grid(yaxtp) {
      std::cout << "Structured grid backend" << std::endl;
    }
    
    GridStruct::~GridStruct() {
    
    }

    int GridStruct::get_size_impl() {
        return m_ext.i * m_ext.j;
    }
    
    void GridStruct::set_subdomain_start_impl(int i, int j) {
        m_start.i = i;
        m_start.j = j;
    }
    
    void GridStruct::set_subdomain_end_impl(int i, int j) {
        m_end.i = i;
        m_end.j = j;
    }
    
    void GridStruct::set_subdomain_ext_impl(int i, int j) {
        m_ext.i = i;
        m_ext.j = j;
    }
    
    void GridStruct::set_subdomain_off_impl(int i, int j) {
        m_off.i = i;
        m_off.j = j;
    }
    
    void GridStruct::set_domain_ext_impl(int i, int j) {
        m_glob.i = i;
        m_glob.j = j;
    }
    
    YaxtRedist GridStruct::set_yaxt_redist_impl(int nmodel, int nlv, bool has_kernel_role) {
        int ig, jg;
        int vol_3d = nlv * (m_end.i - m_start.i + 1) * (m_end.j - m_start.j +1);
        Xt_idxlist idxlist;
        YaxtRedist redist;
        YaxtMap map;
        
        // index list
        std::vector<Xt_int> idx;
        for (int k = 1; k<= nlv; k++) {
            for (int j = m_start.j; j <= m_end.j; j++) {
                jg = m_off.j + j;
                for (int i = m_start.i; i <= m_end.i; i++) {
                    ig = m_off.i + i;
                    idx.push_back(ig + m_glob.i * (jg - 1) + m_glob.i * m_glob.j * (k - 1));
                }
            }
        }
        idxlist = m_yaxt->get_idxlist(idx.data(), vol_3d);
        
        // offsets
        std::vector<int> idx_off;
        int p = 0;
        for (int k = 1; k<= nlv; k++) {
            for (int j = 1; j <= m_ext.j; j++) {
                for (int i = 1; i <= m_ext.i; i++) {
                    if ( (i>=m_start.i && i<=m_end.i) && (j>=m_start.j && j<=m_end.j) )
                        idx_off.push_back(p);
                    p++;
                }
            }
        }
        
        // redistributions
        if (has_kernel_role) {
            map.map_k2m = m_yaxt->get_map(idxlist, nmodel, true );
            map.map_m2k = m_yaxt->get_map(idxlist, nmodel, false);
            
            redist.redist_k2m = m_yaxt->get_redist_off(map.map_k2m, idx_off.data(), true );
            redist.redist_m2k = m_yaxt->get_redist_off(map.map_m2k, idx_off.data(), false);
        } else {
            map.map_k2m = m_yaxt->get_map(idxlist, nmodel, false);
            map.map_m2k = m_yaxt->get_map(idxlist, nmodel, true );
            
            redist.redist_k2m = m_yaxt->get_redist_off(map.map_m2k, idx_off.data(), false);
            redist.redist_m2k = m_yaxt->get_redist_off(map.map_k2m, idx_off.data(), true );
        }
        
        return redist;
        
    }
    
}
