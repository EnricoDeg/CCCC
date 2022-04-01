#include <typeinfo>   // operator typeid

#include "CCCC/data/Variable.hpp"

namespace DKRZ {

    template<typename T>
    Variable<T>::Variable(MPI::Ptr mpip, T* data, int count, bool m2k) {
        m_mpi = mpip;
        m_data = data;
        m_count = count;
        m_m2k = m2k;
        
        if ( typeid(T) == typeid(int) ) {
            m_mpi->get_mpi_datatype(MPI_TYPECLASS_INTEGER, sizeof(T), &m_Tmpi);
        } else if (typeid(T) == typeid(float) or 
                   typeid(T) == typeid(double)) {
            m_mpi->get_mpi_datatype(MPI_TYPECLASS_REAL, sizeof(T), &m_Tmpi);
        } else {
            m_Tmpi = MPI_LOGICAL;
        }
    }

    template Variable<double>::Variable(MPI::Ptr mpip, double *, int, bool);
    template Variable<float>::Variable(MPI::Ptr mpip, float *, int, bool);
    template Variable<int>::Variable(MPI::Ptr mpip, int *, int, bool);

    template<typename T>
    void Variable<T>::exchange(bool cond, bool sender, MPI_Comm intercomm) {
        if (m_m2k == cond)
            m_mpi->exchange(m_data, m_count, sender, m_Tmpi, intercomm);
    }

}
