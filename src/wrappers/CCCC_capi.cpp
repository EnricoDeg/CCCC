#include "CCCC/wrappers/cccc.h"
#include "CCCC/core/CCCC.hpp"

#include <iostream>

namespace DKRZ {
    CCCCt* CCCC_init(MPI_Fint *glob, int nprocs_kernel, char* backend_name) {
        std::cout << "C API, CCCC_init" << std::endl;
        // needed convertions
        std::string backend_name_cpp = backend_name;
        MPI_Comm g = MPI_Comm_f2c(*glob);
        return new CCCC(g, nprocs_kernel, backend_name_cpp);
    }

    MPI_Fint CCCC_local_comm(CCCCt* cccc) {
        MPI_Comm g = cccc->local_comm();
        return MPI_Comm_c2f(g);
    }
    
    void CCCC_finalize(CCCCt* cccc){
        std::cout << "C API, CCCC_finalize" << std::endl;
        delete cccc;
    }
    
    void CCCC_intercomm_create(CCCCt* cccc, int nmodel, int nprocs) {
        std::cout << "C API, CCCC_intercomm_create" << std::endl;
        return cccc->intercomm_create(nmodel, nprocs);
    }

    void CCCC_grid_subdomain_start(CCCCt* cccc, int i, int j) {
        std::cout << "C API, CCCC_grid_subdomain_start" << std::endl;
        return cccc->grid_subdomain_start(i, j);
    }

    void CCCC_grid_subdomain_end(CCCCt* cccc, int i, int j) {
        std::cout << "C API, CCCC_grid_subdomain_end" << std::endl;
        return cccc->grid_subdomain_end(i, j);
    }

    void CCCC_grid_subdomain_ext(CCCCt* cccc, int i, int j) {
        std::cout << "C API, CCCC_grid_subdomain_ext" << std::endl;
        return cccc->grid_subdomain_ext(i, j);
    }

    void CCCC_grid_subdomain_off(CCCCt* cccc, int i, int j) {
        std::cout << "C API, CCCC_grid_subdomain_off" << std::endl;
        return cccc->grid_subdomain_off(i, j);
    }

    void CCCC_grid_domain_ext(CCCCt* cccc, int i, int j) {
        std::cout << "C API, CCCC_grid_domain_ext" << std::endl;
        return cccc->grid_domain_ext(i, j);
    }
    
    void CCCC_start_concurrent(CCCCt* cccc, int nmodel) {
        std::cout << "C API, CCCC_start_concurrent" << std::endl;
        return cccc->start_concurrent(nmodel);
    }
    
    void CCCC_stop_concurrent(CCCCt* cccc, int nmodel) {
        std::cout << "C API, CCCC_stop_concurrent" << std::endl;
        return cccc->stop_concurrent(nmodel);
    }
    
    void CCCC_execute(CCCCt* cccc, int nmodel, int cmd_id) {
        std::cout << "C API, CCCC_execute" << std::endl;
        return cccc->execute(nmodel, cmd_id);
    }
    
    void CCCC_add_command(CCCCt* cccc, void (*Func_ptr) (), int nmodel, int cmd_id) {
        std::cout << "C API, CCCC_add_command" << std::endl;
        return cccc->add_command(Func_ptr, nmodel, cmd_id);
    }
    
    void CCCC_add_field(CCCCt* cccc, double *data, int nlv, int nmodel, int id, int m2k) {
        bool cond = true;
        std::cout << "C API, CCCC_add_field" << std::endl;
        if (m2k == 0) cond = false;
        return cccc->add_field(data, nlv, nmodel, id, cond);
    }
    
    void CCCC_add_variable_double(CCCCt* cccc, double *data, int count, int nmodel, int id, int m2k) {
        bool cond = true;
        std::cout << "C API, CCCC_add_variable" << std::endl;
        if (m2k == 0) cond = false;
        return cccc->add_variable<double>(data, count, nmodel, id, cond);
    }

    void CCCC_add_variable_float(CCCCt* cccc, float *data, int count, int nmodel, int id, int m2k) {
        bool cond = true;
        std::cout << "C API, CCCC_add_variable" << std::endl;
        if (m2k == 0) cond = false;
        return cccc->add_variable<float>(data, count, nmodel, id, cond);
    }

    void CCCC_add_variable_int(CCCCt* cccc, int *data, int count, int nmodel, int id, int m2k) {
        bool cond = true;
        std::cout << "C API, CCCC_add_variable" << std::endl;
        if (m2k == 0) cond = false;
        return cccc->add_variable<int>(data, count, nmodel, id, cond);
    }
    
    void CCCC_exchange_k2m(CCCCt* cccc, int nmodel, int id) {
        std::cout << "C API, CCCC_exchange_k2m" << std::endl;
        return cccc->exchange_k2m(nmodel, id);
    }
    
    void CCCC_exchange_m2k(CCCCt* cccc, int nmodel, int id) {
        std::cout << "C API, CCCC_exchange_m2k" << std::endl;
        return cccc->exchange_m2k(nmodel, id);
    }

    void CCCC_set_redist(CCCCt* cccc, int nmodel, int nlv) {
        std::cout << "C API, CCCC_set_redist" << std::endl;
        return cccc->set_redist(nmodel, nlv);
    }
    
    bool CCCC_has_kernel_role(CCCCt* cccc) {
        return cccc->has_kernel_role();
    }
    
}
