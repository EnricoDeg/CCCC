module func_pointer_mod
   use, intrinsic :: iso_c_binding

   implicit none

contains

    subroutine hello_dummy() bind(C)
        write(*,*) "hello"
    end subroutine hello_dummy

    subroutine world_dummy() bind(C)
        write(*,*) "world"
    end subroutine world_dummy

end module func_pointer_mod

program test
    USE mpi
    use libcccc
    use, intrinsic :: iso_c_binding
    use func_pointer_mod
    implicit none
    type(cccc) :: f
    integer i, ierr;
    character(len=100) :: backend

    call MPI_Init(ierr)

    backend = "structured"
    ! Create an object of type cccc
    f = cccc(MPI_COMM_WORLD, 1, trim(backend))

    i = f%intercomm_create(1, 1)

    call f%add_command(c_funloc(hello_dummy), 1, 1)
    call f%add_command(c_funloc(world_dummy), 1, 1)

    call f%start_concurrent(1)
    if (.not. f%has_kernel_role()) then
      write(*,*) "Model procs print this string"
      call f%execute(1,1)
    end if
    call f%stop_concurrent(1)

    call f%finalize

    call MPI_Finalize(ierr)

!contains

!    subroutine hello_dummy(x) bind(C, name='hello_dummy')
!      real (c_float), VALUE, intent (in) :: x
!        write(*,*) "hello world"
!    end subroutine hello_dummy

end program
