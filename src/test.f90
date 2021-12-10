program test
    USE mpi
    use libcccc
    implicit none
    type(cccc) :: f
    integer i;

    ! Create an object of type cccc
    f = cccc(1, 1)

    i = f%intercomm_create(1)

    call f%finalize
end program