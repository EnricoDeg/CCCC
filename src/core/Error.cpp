#include "CCCC/core/Error.hpp"

namespace DKRZ {
    // error handler
    void handle_error(std::string msg) {
        std::cerr << "CCCC Error: " << msg << std::endl;
        std::exit (EXIT_FAILURE);
    }
}
