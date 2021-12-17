module libcccc
    use iso_c_binding

    private
    public :: CCCC

    include "CCCC_cdef.f90"

    ! We'll use a Fortan type to represent a C++ class here, in an opaque maner
    type CCCC
        private
        type(c_ptr) :: ptr ! pointer to the CCCC class
    contains
        procedure :: finalize => CCCC_finalize_polymorph ! Destructor for gfortran
        ! Function member
        procedure :: intercomm_create => CCCC_intercomm_create
        procedure :: start_concurrent => CCCC_start_concurrent
        procedure :: stop_concurrent  => CCCC_stop_concurrent
        procedure :: has_kernel_role  => CCCC_has_kernel_role
    end type

    ! This function acts as the constructor for cccc type
    interface CCCC
        procedure CCCC_init
    end interface

contains
    function CCCC_init(glob, nprocs_kernel, backend)
        implicit none
        type(cccc) :: CCCC_init
        integer :: glob
        integer, intent(in) :: nprocs_kernel
        character(len=*) :: backend

        character(kind=C_char), dimension(len(backend)+1) :: c_backend
        integer :: i

        do i = 1, len(backend)
          c_backend(i) = backend(i:i)
        end do
        c_backend(len(backend)+1) = c_null_char

        cccc_init%ptr = CCCC_init_c(glob, nprocs_kernel, c_backend)
    end function

    subroutine CCCC_finalize(this)
        implicit none
        type(cccc) :: this
        call cccc_finalize_c(this%ptr)
    end subroutine

    ! Bounds procedure needs to take a polymorphic (class) argument
    subroutine CCCC_finalize_polymorph(this)
        implicit none
        class(cccc) :: this
        call cccc_finalize_c(this%ptr)
    end subroutine

    integer function CCCC_intercomm_create(this, nmodel, nprocs)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        integer, intent(in) :: nprocs
        CCCC_intercomm_create = CCCC_intercomm_create_c(this%ptr, nmodel, nprocs)
    end function

    subroutine CCCC_start_concurrent(this, nmodel)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        call CCCC_start_concurrent_c(this%ptr, nmodel)
    end subroutine

    subroutine CCCC_stop_concurrent(this, nmodel)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        call CCCC_stop_concurrent_c(this%ptr, nmodel)
    end subroutine

    logical function CCCC_has_kernel_role(this)
        implicit none
        class(cccc) :: this
        CCCC_has_kernel_role = CCCC_has_kernel_role_c(this%ptr)
    end function

end module
