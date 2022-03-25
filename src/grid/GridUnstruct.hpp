#ifndef _CCCCGRIDUNSTRUCT_HPP_
#define _CCCCGRIDUNSTRUCT_HPP_

#include "Grid.hpp"

namespace DKRZ {

    //! Derived class for Unstructured Grid type
    /*!

    */
    class GridUnstruct : public Grid {

        public:
            GridUnstruct(Yaxt::Ptr yaxtp);
            virtual ~GridUnstruct();

        protected:

        private:

    };

}

#endif
