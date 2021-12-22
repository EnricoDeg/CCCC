#include "CCCC/grid/Grid.hpp"

namespace DKRZ {

    Grid::Grid(Yaxt::Ptr yaxtp) : m_yaxt(yaxtp) {
    }

    Grid::~Grid() {
    }

    void Grid::set_subdomain_start(int i, int j) {
        this->set_subdomain_start_impl(i,j);
    }

    void Grid::set_subdomain_end(int i, int j) {
        this->set_subdomain_end_impl(i,j);
    }

    void Grid::set_subdomain_ext(int i, int j) {
        this->set_subdomain_ext_impl(i,j);
    }

    void Grid::set_subdomain_off(int i, int j) {
        this->set_subdomain_off_impl(i,j);
    }

    void Grid::set_domain_ext(int i, int j) {
        this->set_domain_ext_impl(i,j);
    }

    YaxtRedist Grid::set_yaxt_redist(int nmodel, int nlv, bool has_kernel_role) {
        return this->set_yaxt_redist_impl(nmodel, nlv, has_kernel_role);
    }

    // implemementations
    void  Grid::set_subdomain_start_impl(int i, int j) {
        handle_error("set_subdomain_start() not supported for selected grid type");
    }

    void  Grid::set_subdomain_end_impl(int i, int j) {
        handle_error("set_subdomain_end() not supported for selected grid type");
    }

    void  Grid::set_subdomain_ext_impl(int i, int j) {
        handle_error("set_subdomain_ext() not supported for selected grid type");
    }

    void  Grid::set_subdomain_off_impl(int i, int j) {
        handle_error("set_subdomain_off() not supported for selected grid type");
    }

    void  Grid::set_domain_ext_impl(int i, int j) {
        handle_error("set_domain_ext() not supported for selected grid type");
    }

    YaxtRedist Grid::set_yaxt_redist_impl(int nmodel, int nlv, bool has_kernel_role) {
        handle_error("set_yaxt_redist() not supported for selected grid type");
    }

}
