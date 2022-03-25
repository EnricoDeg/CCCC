#ifndef _CCCCGRIDGAUSS_HPP_
#define _CCCCGRIDGAUSS_HPP_

#include "Grid.hpp"

namespace DKRZ {

    //! Derived class for Gaussian Grid type
    /*!
      
    */
    class GridGauss : public Grid {

        public:
            GridGauss(Yaxt::Ptr yaxtp);
            virtual ~GridGauss();

        protected:

        private:

    };

}

#endif
