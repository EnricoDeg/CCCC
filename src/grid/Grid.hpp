#ifndef _CCCCGRIDWRAPPER_HPP_
#define _CCCCGRIDWRAPPER_HPP_
#include <memory>

#include "CCCC/grid/Grid_Flags.hpp"

namespace DKRZ {

    class Grid {
        public:
        typedef std::shared_ptr<Grid> Ptr;
        Grid();
        ~Grid();
    };

}
#endif
