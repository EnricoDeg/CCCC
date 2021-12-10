module libcccc
    use iso_c_binding

    private
    public :: CCCC

    ! Yes, include is a keyword in Fortran !
    include "CCCC_cdef.f90"

    ! We'll use a Fortan type to represent a C++ class here, in an opaque maner
    type CCCC
        private
        type(c_ptr) :: ptr ! pointer to the Foo class
    contains
        ! We can bind some functions to this type, allowing for a cleaner syntax.
!#ifdef __GNUC__
        procedure :: finalize => CCCC_finalize_polymorph ! Destructor for gfortran
!#else
!        final :: cccc_finalize ! Destructor
!#endif
        ! Function member
        procedure :: intercomm_create => CCCC_intercomm_create
    end type

    ! This function will act as the constructor for cccc type
    interface CCCC
        procedure CCCC_init
    end interface

contains ! Implementation of the functions. We just wrap the C function here.
    function CCCC_init(glob, nprocs_kernel)
        implicit none
        type(cccc) :: CCCC_init
        integer, intent(in) :: glob, nprocs_kernel
        cccc_init%ptr = CCCC_init_c(glob, nprocs_kernel)
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

end module
