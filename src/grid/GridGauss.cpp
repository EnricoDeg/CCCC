#include <iostream>

#include "GridGauss.hpp"

namespace DKRZ {

    GridGauss::GridGauss(Yaxt::Ptr yaxtp) : Grid(yaxtp) {
      std::cout << "Gaussian grid backend" << std::endl;
    }

    GridGauss::~GridGauss() {

    }

}
