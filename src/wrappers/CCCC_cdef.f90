! C functions declaration
interface
    function CCCC_init_c(glob, nprocs_kernel, backend) bind(C, name="CCCC_init")
        use iso_c_binding
        implicit none
        type(c_ptr) :: CCCC_init_c
        integer :: glob
        integer(c_int), value :: nprocs_kernel
        character(kind=C_char), dimension(*) :: backend
    end function

    function CCCC_local_comm_c(cccc) bind(C, name="CCCC_local_comm")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer :: CCCC_local_comm_c
    end function

    subroutine CCCC_finalize_c(cccc) bind(C, name="CCCC_finalize")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
    end subroutine

    subroutine CCCC_intercomm_create_c(cccc, nmodel, nprocs) bind(C, name="CCCC_intercomm_create")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
        integer(c_int), value :: nprocs
    end subroutine

    subroutine CCCC_start_concurrent_c(cccc, nmodel) bind(C, name="CCCC_start_concurrent")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
    end subroutine

    subroutine CCCC_stop_concurrent_c(cccc, nmodel) bind(C, name="CCCC_stop_concurrent")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
    end subroutine

    function CCCC_has_kernel_role_c(cccc) bind (C, name='CCCC_has_kernel_role')
        use iso_c_binding
        implicit none
        logical(c_bool) :: CCCC_has_kernel_role_c
        type(c_ptr), value :: cccc
    end function

    subroutine CCCC_execute_c(cccc, nmodel, cmd_id) bind(C, name='CCCC_execute')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
        integer(c_int), value :: cmd_id
    end subroutine

    subroutine CCCC_add_command_c(cccc, func_ptr, nmodel, cmd_id) bind(C, name='CCCC_add_command')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        type (c_funptr), VALUE, intent (in) :: func_ptr
        integer(c_int), value :: nmodel
        integer(c_int), value :: cmd_id
    end subroutine

    subroutine CCCC_add_field_c(cccc, data, nlv, nmodel, id, m2k) bind(C, name='CCCC_add_field')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        real(c_double), dimension(*), intent(inout) :: data
        integer(c_int), value :: nlv
        integer(c_int), value :: nmodel
        integer(c_int), value :: id
        integer(c_int), value :: m2k
    end subroutine CCCC_add_field_c

    subroutine CCCC_add_variable_c(cccc, data, count, nmodel, id, m2k) bind(C, name='CCCC_add_variable')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        real(c_double), dimension(*), intent(inout) :: data
        integer(c_int), value :: count
        integer(c_int), value :: nmodel
        integer(c_int), value :: id
        integer(c_int), value :: m2k
    end subroutine CCCC_add_variable_c

    subroutine CCCC_exchange_k2m_c(cccc, nmodel, id) bind(C, name='CCCC_exchange_k2m')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
        integer(c_int), value :: id
    end subroutine CCCC_exchange_k2m_c

    subroutine CCCC_exchange_m2k_c(cccc, nmodel, id) bind(C, name='CCCC_exchange_m2k')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
        integer(c_int), value :: id
    end subroutine CCCC_exchange_m2k_c

    subroutine CCCC_grid_subdomain_start_c(cccc, i, j) bind(C, name='CCCC_grid_subdomain_start')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: i
        integer(c_int), value :: j
    end subroutine CCCC_grid_subdomain_start_c

    subroutine CCCC_grid_subdomain_end_c(cccc, i, j) bind(C, name='CCCC_grid_subdomain_end')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: i
        integer(c_int), value :: j
    end subroutine CCCC_grid_subdomain_end_c

    subroutine CCCC_grid_subdomain_ext_c(cccc, i, j) bind(C, name='CCCC_grid_subdomain_ext')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: i
        integer(c_int), value :: j
    end subroutine CCCC_grid_subdomain_ext_c

    subroutine CCCC_grid_subdomain_off_c(cccc, i, j) bind(C, name='CCCC_grid_subdomain_off')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: i
        integer(c_int), value :: j
    end subroutine CCCC_grid_subdomain_off_c

    subroutine CCCC_grid_domain_ext_c(cccc, i, j) bind(C, name='CCCC_grid_domain_ext')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: i
        integer(c_int), value :: j
    end subroutine CCCC_grid_domain_ext_c
    
    subroutine CCCC_set_redist_c(cccc, nmodel, nlv) bind(C, name='CCCC_set_redist')
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
        integer(c_int), value :: nlv
    end subroutine CCCC_set_redist_c
    
end interface
