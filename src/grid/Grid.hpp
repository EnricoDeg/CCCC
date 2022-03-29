#ifndef _CCCCGRIDWRAPPER_HPP_
#define _CCCCGRIDWRAPPER_HPP_
#include <memory>

#include "CCCC/grid/Grid_Flags.hpp"
#include "CCCC/yaxt/Yaxt.hpp"
#include "CCCC/core/Error.hpp"

namespace DKRZ {
    
    //! C structure for a 2D point coordinates
    /*!

    */
    struct Point2DType {
        int i;
        int j;
    };
    
    //! Parent class for supported grids
    /*!
      
    */
    class Grid {
        public:
            typedef std::shared_ptr<Grid> Ptr;
            Grid(Yaxt::Ptr yaxtp);
            ~Grid();
            int get_size();
            void set_subdomain_start(int i, int j);
            void set_subdomain_end(int i, int j);
            void set_subdomain_ext(int i, int j);
            void set_subdomain_off(int i, int j);
            void set_domain_ext(int i, int j);
            YaxtRedist set_yaxt_redist(int nmodel, int nlv, bool has_kernel_role);
            
        protected:
            // implementations
            virtual int get_size_impl();
            virtual void set_subdomain_start_impl(int i, int j);
            virtual void set_subdomain_end_impl(int i, int j);
            virtual void set_subdomain_ext_impl(int i, int j);
            virtual void set_subdomain_off_impl(int i, int j);
            virtual void set_domain_ext_impl(int i, int j);
            virtual YaxtRedist set_yaxt_redist_impl(int nmodel, int nlv, bool has_kernel_role);
            
            // variables used by all derived classes
            Yaxt::Ptr m_yaxt;
    };
    
}
#endif
