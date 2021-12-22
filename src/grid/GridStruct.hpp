#ifndef _CCCCGRIDSTRUCT_HPP_
#define _CCCCGRIDSTRUCT_HPP_

#include "Grid.hpp"

namespace DKRZ {

    class GridStruct : public Grid {

        public:
            GridStruct(Yaxt::Ptr yaxtp);
            virtual ~GridStruct();

        protected:
            void set_subdomain_start_impl(int i, int j); 
            void set_subdomain_end_impl(int i, int j);
            void set_subdomain_ext_impl(int i, int j);
            void set_subdomain_off_impl(int i, int j);
            void set_domain_ext_impl(int i, int j);
            YaxtRedist set_yaxt_redist_impl(int nmodel, int nlv, bool has_kernel_role);

        private:
            Point2DType m_start;
            Point2DType m_end;
            Point2DType m_ext;
            Point2DType m_off;
            Point2DType m_glob;

    };

}

#endif
