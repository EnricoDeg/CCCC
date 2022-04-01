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
        procedure :: local_comm           => CCCC_local_comm
        procedure :: intercomm_create     => CCCC_intercomm_create
        procedure :: grid_subdomain_start => CCCC_grid_subdomain_start
        procedure :: grid_subdomain_end   => CCCC_grid_subdomain_end
        procedure :: grid_subdomain_off   => CCCC_grid_subdomain_off
        procedure :: grid_subdomain_ext   => CCCC_grid_subdomain_ext
        procedure :: grid_domain_ext      => CCCC_grid_domain_ext
        procedure :: start_concurrent     => CCCC_start_concurrent
        procedure :: stop_concurrent      => CCCC_stop_concurrent
        procedure :: execute              => CCCC_execute
        procedure :: has_kernel_role      => CCCC_has_kernel_role
        procedure :: add_command          => CCCC_add_command
        procedure :: add_field            => CCCC_add_field
        procedure :: add_variable_double  => CCCC_add_variable_double
        procedure :: add_variable_float   => CCCC_add_variable_float
        procedure :: add_variable_int     => CCCC_add_variable_int
        generic   :: add_variable         => add_variable_double, &
                                             add_variable_float,  &
                                             add_variable_int
        procedure :: exchange_k2m         => CCCC_exchange_k2m
        procedure :: exchange_m2k         => CCCC_exchange_m2k
        procedure :: set_redist           => CCCC_set_redist
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
    
    integer function CCCC_local_comm(this)
        implicit none
        class(cccc) :: this
        CCCC_local_comm = CCCC_local_comm_c(this%ptr)
    end function
    
    subroutine CCCC_intercomm_create(this, nmodel, nprocs)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        integer, intent(in) :: nprocs
        call CCCC_intercomm_create_c(this%ptr, nmodel, nprocs)
    end subroutine
    
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
    
    subroutine CCCC_execute(this, nmodel, cmd_id)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        integer, intent(in) :: cmd_id
        call CCCC_execute_c(this%ptr, nmodel, cmd_id)
    end subroutine 
    
    logical function CCCC_has_kernel_role(this)
        implicit none
        class(cccc) :: this
        CCCC_has_kernel_role = CCCC_has_kernel_role_c(this%ptr)
    end function
    
    subroutine CCCC_add_command(this, func_ptr, nmodel, cmd_id)
        implicit none
        class(cccc) :: this
        type (c_funptr), intent (in) :: func_ptr
        integer, intent(in) :: nmodel
        integer, intent(in) :: cmd_id
        call CCCC_add_command_c(this%ptr, func_ptr, nmodel, cmd_id)
    end subroutine
    
    subroutine CCCC_add_field(this, data, nlv, nmodel, id, m2k)
        implicit none
        class(cccc) :: this
        real(c_double), dimension(*), intent(inout) :: data
        integer, intent(in) :: nlv
        integer, intent(in) :: nmodel
        integer, intent(in) :: id
        logical, intent(in) :: m2k
        integer :: cond
        if (m2k .eqv. .false.) then
            cond = 0
        else
            cond = 1
        end if
        call CCCC_add_field_c(this%ptr, data, nlv, nmodel, id, cond)
    end subroutine
    
    subroutine CCCC_add_variable_double(this, data, count, nmodel, id, m2k)
        implicit none
        class(cccc) :: this
        real(c_double), dimension(*), intent(inout) :: data
        integer, intent(in) :: count
        integer, intent(in) :: nmodel
        integer, intent(in) :: id
        logical, intent(in) :: m2k
        integer :: cond
        if (m2k .eqv. .false.) then
            cond = 0
        else
            cond = 1
        end if
        call CCCC_add_variable_double_c(this%ptr, data, count, nmodel, id, cond)
    end subroutine

    subroutine CCCC_add_variable_float(this, data, count, nmodel, id, m2k)
        implicit none
        class(cccc) :: this
        real(c_float), dimension(*), intent(inout) :: data
        integer, intent(in) :: count
        integer, intent(in) :: nmodel
        integer, intent(in) :: id
        logical, intent(in) :: m2k
        integer :: cond
        if (m2k .eqv. .false.) then
            cond = 0
        else
            cond = 1
        end if
        call CCCC_add_variable_float_c(this%ptr, data, count, nmodel, id, cond)
    end subroutine

    subroutine CCCC_add_variable_int(this, data, count, nmodel, id, m2k)
        implicit none
        class(cccc) :: this
        integer(c_int), dimension(*), intent(inout) :: data
        integer, intent(in) :: count
        integer, intent(in) :: nmodel
        integer, intent(in) :: id
        logical, intent(in) :: m2k
        integer :: cond
        if (m2k .eqv. .false.) then
            cond = 0
        else
            cond = 1
        end if
        call CCCC_add_variable_int_c(this%ptr, data, count, nmodel, id, cond)
    end subroutine
    
    subroutine CCCC_exchange_k2m(this, nmodel, id)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        integer, intent(in) :: id
        call CCCC_exchange_k2m_c(this%ptr, nmodel, id)
    end subroutine
    
    subroutine CCCC_exchange_m2k(this, nmodel, id)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        integer, intent(in) :: id
        call CCCC_exchange_m2k_c(this%ptr, nmodel, id)
    end subroutine
    
    subroutine CCCC_grid_subdomain_start(this, i, j)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: i
        integer, intent(in) :: j
        call CCCC_grid_subdomain_start_c(this%ptr, i, j)
    end subroutine CCCC_grid_subdomain_start

    subroutine CCCC_grid_subdomain_end(this, i, j)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: i
        integer, intent(in) :: j
        call CCCC_grid_subdomain_end_c(this%ptr, i, j)
    end subroutine CCCC_grid_subdomain_end

    subroutine CCCC_grid_subdomain_off(this, i, j)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: i
        integer, intent(in) :: j
        call CCCC_grid_subdomain_off_c(this%ptr, i, j)
    end subroutine CCCC_grid_subdomain_off

    subroutine CCCC_grid_subdomain_ext(this, i, j)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: i
        integer, intent(in) :: j
        call CCCC_grid_subdomain_ext_c(this%ptr, i, j)
    end subroutine CCCC_grid_subdomain_ext

    subroutine CCCC_grid_domain_ext(this, i, j)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: i
        integer, intent(in) :: j
        call CCCC_grid_domain_ext_c(this%ptr, i, j)
    end subroutine CCCC_grid_domain_ext
    
    subroutine CCCC_set_redist(this, nmodel, nlv)
        implicit none
        class(cccc) :: this
        integer, intent(in) :: nmodel
        integer, intent(in) :: nlv
        call CCCC_set_redist_c(this%ptr, nmodel, nlv)
    end subroutine CCCC_set_redist
    
end module
