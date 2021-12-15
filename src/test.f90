program test
    USE mpi
    use libcccc
    implicit none
    type(cccc) :: f
    integer i, ierr;
    character(len=100) :: backend

    call MPI_Init(ierr)

    backend = "structured"
    ! Create an object of type cccc
    f = cccc(MPI_COMM_WORLD, 1, trim(backend))

    !i = f%intercomm_create(1, 1)

    call f%finalize

    call MPI_Finalize(ierr)

end program
