! C functions declaration
interface
    function CCCC_init_c(glob, nprocs_kernel) bind(C, name="CCCC_init")
        use iso_c_binding
        implicit none
        type(c_ptr) :: CCCC_init_c
        integer(c_int), value :: glob
        integer(c_int), value :: nprocs_kernel
    end function

    subroutine CCCC_finalize_c(cccc) bind(C, name="CCCC_finalize")
        use iso_c_binding
        implicit none
        type(c_ptr), value :: cccc
    end subroutine

    function CCCC_intercomm_create_c(cccc, nmodel) bind(C, name="CCCC_intercomm_create")
        use iso_c_binding
        implicit none
        integer(c_int) :: CCCC_intercomm_create_c
        type(c_ptr), intent(in), value :: cccc
        integer(c_int), value :: nmodel
    end function

end interface