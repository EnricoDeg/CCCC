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

end interface
