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

    subroutine CCCC_finalize_c(cccc) bind(C, name="CCCC_finalize")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
    end subroutine

    function CCCC_intercomm_create_c(cccc, nmodel, nprocs) bind(C, name="CCCC_intercomm_create")
        use iso_c_binding
        implicit none
        integer(c_int) :: CCCC_intercomm_create_c
        type(c_ptr), value :: cccc
        integer(c_int), value :: nmodel
        integer(c_int), value :: nprocs
    end function

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

end interface
