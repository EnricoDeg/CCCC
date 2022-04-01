module func_pointer_mod

   use, intrinsic :: iso_c_binding
   use mpi
   implicit none
   private
   double precision, allocatable, dimension(:,:) :: a
   integer :: idx
   integer, parameter :: N = 3
   PUBLIC :: allocate_variable, initialize_variable, compute_variable, &
             deallocate_variable
contains

    subroutine hello_dummy() bind(C)
        write(*,*) "hello"
    end subroutine hello_dummy

    subroutine world_dummy() bind(C)
        write(*,*) "world"
    end subroutine world_dummy

    subroutine allocate_variable() bind(C)
        write(*,*) "allocation"
        allocate(a(N,N))
    end subroutine allocate_variable

    subroutine initialize_variable() bind(C)
        integer :: i, j
        double precision :: ts, te

        write(*,*) "initialization"
        idx = 0
        ts = MPI_Wtime()
        do j = 1,N
            do i = 1,N
                a(i,j) = 1.0e0
            end do
        end do
        te = MPI_Wtime()
        write(*,*) "timer = ", te - ts
    end subroutine initialize_variable

    SUBROUTINE compute_variable() bind(C)
        idx = idx + 1
        a(:,:) = a(:,:) + idx
    END SUBROUTINE compute_variable

    SUBROUTINE deallocate_variable() bind(C)
        deallocate(a)
    END SUBROUTINE deallocate_variable

end module func_pointer_mod

program test

    USE mpi
    use libcccc
    use, intrinsic :: iso_c_binding
    use func_pointer_mod
    implicit none

    integer, parameter :: dp = selected_real_kind(15)
    type(cccc) :: f
    integer ierr;
    character(len=100) :: backend
    double precision, target, allocatable, dimension(:,:) :: aaa
    double precision, target, allocatable, dimension(:,:,:) :: bbb
    real(dp), target, allocatable, dimension(:) :: z
    integer , target, allocatable, dimension(:) :: w
    integer :: myid, t, i, j, k
    integer, parameter :: ND = 10
    integer, parameter :: SL = 1 ! surface level
    integer, parameter :: VL = 2 ! vertical levels
    integer, parameter :: VAR_KERNEL = 1 ! VAR kernel ID
    integer, parameter :: NPROCS_MAIN = 2 ! MPI tasks model
    integer, parameter :: NPROCS_VAR_KERNEL = 2 ! MPI tasks VAR kernel
    integer, parameter :: CMD_INIT_ID = 1
    integer, parameter :: CMD_COMP_ID = 2
    integer, parameter :: CMD_FIN_ID  = 3
    integer, parameter :: EXG_ID = 1
    integer, parameter :: NSTEPS = 10 ! Time loop steps

    call MPI_Init(ierr)
    call MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)

    backend = "structured" ! model grid type
    ! Create an object of type cccc
    f = cccc(MPI_COMM_WORLD, NPROCS_MAIN, trim(backend))

    call f%intercomm_create(VAR_KERNEL, NPROCS_VAR_KERNEL)

    ! field allocated and initialized
    allocate(aaa(ND,ND), bbb(ND,ND,VL))
    aaa(:,:) = 0.0e0
    bbb(:,:,:) = 0.0e0

    ! initialize grid info
    CALL f%grid_subdomain_ext(ND, ND)

    ! add fields 
    CALL f%add_field(aaa, SL, VAR_KERNEL, EXG_ID, .true.) ! .true. means m2k
    CALL f%add_field(bbb, VL, VAR_KERNEL, EXG_ID, .true.) ! .true. means m2k

    ! variable allocated and initialized
    allocate(z(1), w(1))
    z = 0.0e0
    w = -1

    ! add commands
    call f%add_command(c_funloc(allocate_variable)  , VAR_KERNEL, CMD_INIT_ID)
    call f%add_command(c_funloc(initialize_variable), VAR_KERNEL, CMD_INIT_ID)
    call f%add_command(c_funloc(compute_variable)   , VAR_KERNEL, CMD_COMP_ID)
    call f%add_command(c_funloc(deallocate_variable), VAR_KERNEL, CMD_FIN_ID )

    ! add variables
    call f%add_variable(z, SIZE(z), VAR_KERNEL, 1, .true.) ! .true. means m2k
    call f%add_variable(w, SIZE(w), VAR_KERNEL, 1, .true.) ! .true. means m2k

    ! CONCURRENT EXECUTION BEGIN
    call f%start_concurrent(VAR_KERNEL)
    if (.not. f%has_kernel_role()) then
      ! variables and fields modified during main model time loop and used 
      ! as input for the kernel duyring each time step
      z = 1.0e0
      w = 0
      do j = 1,ND
        do i = 1,ND
          aaa(i,j) = 1.0e0 * i * j * (myid + 1)
        end do
      end do
      do k = 1,VL
        do j = 1,ND
          do i = 1,ND
            bbb(i,j,k) = 2.0e0 * i * j * k * (myid + 1)
          end do
        end do
      end do

      ! initialize the kernel (fields only allocated on kernel procs)
      call f%execute(VAR_KERNEL, CMD_INIT_ID)
      ! Time loop
      do t = 1,NSTEPS
        call f%exchange_m2k(VAR_KERNEL, EXG_ID)
        call f%execute(VAR_KERNEL, CMD_COMP_ID)
        aaa = aaa + t
        bbb = bbb + 2 * t
        z   = z   + t / 2.0e0
        w   = w   - t
      end do
      call f%execute(VAR_KERNEL, CMD_FIN_ID)
    end if
    call f%stop_concurrent(VAR_KERNEL)
    ! CONCURRENT EXECUTION END

    ! destroy cccc type object
    call f%finalize

    call MPI_Finalize(ierr)

end program
