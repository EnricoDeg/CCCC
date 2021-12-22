#include <iostream>

#include "GridUnstruct.hpp"

namespace DKRZ {

    GridUnstruct::GridUnstruct(Yaxt::Ptr yaxtp) : Grid(yaxtp) {
      std::cout << "Unstructured grid backend" << std::endl;
    }

    GridUnstruct::~GridUnstruct() {

    }

}
