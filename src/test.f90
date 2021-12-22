module func_pointer_mod

   use, intrinsic :: iso_c_binding
   use mpi
   implicit none

   double precision, allocatable, dimension(:,:) :: a
   integer, parameter :: N = 1000

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
        
        ts = MPI_Wtime()
        do j = 1,N
            do i = 1,N
                a(i,j) = 1.0e0
            end do
        end do
        te = MPI_Wtime()
        write(*,*) "timer = ", te - ts
    end subroutine initialize_variable

end module func_pointer_mod

program test

    USE mpi
    use libcccc
    use, intrinsic :: iso_c_binding
    use func_pointer_mod
    implicit none

    integer, parameter :: dp = selected_real_kind(15)
    type(cccc) :: f
    integer i, ierr;
    character(len=100) :: backend
    real(dp), target, allocatable, dimension(:) :: z

    call MPI_Init(ierr)

    backend = "structured"
    ! Create an object of type cccc
    f = cccc(MPI_COMM_WORLD, 1, trim(backend))

    call f%intercomm_create(1, 1)

    allocate(z(1))

    call f%add_command(c_funloc(allocate_variable), 1, 1)
    call f%add_command(c_funloc(initialize_variable), 1, 1)

    z = 0.0e0
    call f%add_variable(z, 1, 1, 1, .true.)

    call f%start_concurrent(1)
    if (.not. f%has_kernel_role()) then
      z = 1.0e0
      write(*,*) "Model procs print this string"
      call f%exchange_m2k(1, 1)
      call f%execute(1,1)
    end if
    call f%stop_concurrent(1)

    write(*,*) "z = ", z

    call f%finalize

    call MPI_Finalize(ierr)

end program
