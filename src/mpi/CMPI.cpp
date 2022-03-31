#include "CCCC/mpi/CMPI.hpp"

namespace DKRZ {

    MPI::MPI(MPI_Comm glob) {
        m_glob_comm = glob;
        MPI_Comm_rank(m_glob_comm, &m_glob_rank);
        m_local_comm = MPI_COMM_NULL;
    }

    MPI::~MPI() {

    }

    MPI_Comm MPI::intercomm_create(int nmodel, int nprocs, std::vector<int> nprocs_all) {

        int lb = 0;
        for (int i = 0; i < nmodel; i++)
            lb += nprocs_all[i];

        int ikey, icolor, ilocal_root, iremote_root;
        ikey = m_glob_rank;
        if (m_glob_rank < nprocs_all[0]) {
            icolor = 1;
            ilocal_root = 0;
            iremote_root = lb;
        } else if (m_glob_rank >= lb  && m_glob_rank < lb+nprocs) {
            icolor = 2;
            ilocal_root = 0;
            iremote_root = 0;
            m_mymodel = nmodel;
        } else {
            icolor = 3;
            ilocal_root = 0;
            iremote_root = -1;
        }
        MPI_Comm local_comm, intercomm;
        MPI_Comm_split(m_glob_comm, icolor, ikey, &local_comm);
        if (iremote_root != -1) {
            MPI_Intercomm_create(local_comm, ilocal_root,
                                 m_glob_comm, iremote_root,
                                 0, &intercomm);
            if (m_local_comm == MPI_COMM_NULL) {
                m_local_comm = local_comm;
                MPI_Comm_rank(m_local_comm, &m_local_rank);
            }
        }
        return intercomm;

    }

    int MPI::get_cmd(MPI_Comm intercomm) {
        int i_cmd;
        MPI_Status stat;
        if (m_local_rank == 0) {
            MPI_Recv(&i_cmd, 1, MPI_INT, 0, 0, intercomm, &stat);
        }
        MPI_Bcast(&i_cmd, 1, MPI_INT, 0, m_local_comm);
        return i_cmd;
    }

    void MPI::send_cmd(int cmd, MPI_Comm intercomm) {
        if (m_local_rank == 0) {
            MPI_Send(&cmd, 1, MPI_INT, 0, 0, intercomm);
        }
    }

    void MPI::send(int nmodel, double *data, int count, MPI_Comm intercomm) {
        if (m_local_rank == 0) {
            MPI_Send(data, count, MPI_DOUBLE, 0, 0, intercomm);
        }
    }

    void MPI::recv(int nmodel, double *data, int count, MPI_Comm intercomm) {
        if (m_local_rank == 0) {
            MPI_Status stat;
            MPI_Recv(data, count, MPI_DOUBLE, 0, 0, intercomm, &stat);
        }
        MPI_Bcast(&data, count, MPI_DOUBLE, 0, m_local_comm);
    }

    void MPI::exchange(double *data, int size, bool sender, MPI_Comm intercomm) {
        if (sender) {
            MPI_Send(data, size, MPI_DOUBLE, m_local_rank, 0, intercomm);
        } else {
            MPI_Status stat;
            MPI_Recv(data, size, MPI_DOUBLE, m_local_rank, 0, intercomm, &stat);
        }
    }

    int MPI::mymodel() {
        return m_mymodel;
    }

    MPI_Comm MPI::local_comm() {
        return m_local_comm;
    }

}
