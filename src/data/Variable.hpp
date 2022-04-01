#include "CCCC/mpi/CMPI.hpp"

namespace DKRZ {

  class VariableBase{
  public:
      VariableBase() {};
      virtual void exchange(bool cond, bool sender, MPI_Comm intercomm) = 0;
  };

  template<typename T>
  class Variable : public VariableBase {
  public:
      Variable(MPI::Ptr mpip, T* data, int count, bool m2k);
      ~Variable();
      void exchange(bool cond, bool sender, MPI_Comm intercomm);
  private:
      MPI::Ptr  m_mpi;
      T* m_data;
      int m_count;
      bool m_m2k;
      MPI_Datatype m_Tmpi;
  };

}
