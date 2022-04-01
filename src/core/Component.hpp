#include <vector>
#include <map>

#include "CCCC/grid/Grid.hpp"
#include "CCCC/data/Variable.hpp"

#define CCCC_CMD_EXIT -1
#define CCCC_K2M -2
#define CCCC_M2K -3

namespace DKRZ {

  typedef void (*fPtr)();

  //! C structure for a single field
  /*!
    \param data pointer to the field data
    \param redist pointer to the YAXT redistribution
    \param exchange_id exchange ID
    \param size allocated size of the field
    \param m2k exchange direction
  */
  struct FieldType {
    double *data;
    Xt_redist *redist;
    int size;
    bool m2k;
  };

  //! C structure for a single variable
  /*!
    \param data pointer to the variable data
    \param count allocated size for the variable
    \param exchange_id exchange ID
    \param m2k exchange direction
  */
//  struct VariableType {
//    double *data;
//    int count;
//    bool m2k;
//  };

  //! C structure for a single command
  /*!
    \param cmd pointer to the function (command) to be executed
    \param cmd_id command ID
  */
  struct CmdType {
    fPtr cmd;
  };

  //!  library main class
  /*!
    It is the interface between CCCC library internals and the external
    model using the library.
  */
  class Component {
    public:
        typedef std::shared_ptr<Component> Ptr;
        Component(MPI_Comm intercomm, Yaxt::Ptr yaxtp, MPI::Ptr mpip);
        ~Component();
        void add_field_impl(double *data, Xt_redist * rds, int size, int id, bool m2k);
        template <typename T> void add_variable_impl(T *data, int count, int id, bool m2k);
        void add_command_impl(void (*Func_ptr) (), int cmd_id);
        void exchange_field_yaxt_impl(int id, bool cond);
        void exchange_variable_mpi_impl(int id, bool cond, bool sender);
        void exchange_field_mpi_impl(int id, bool cond, bool sender);
        void send_cmd(int cmd_id);
        void remote_loop_impl();

    protected:
        //!
        /*!
          Pointer to a Yaxt class object, which is initialized by the CCCC class constructor.
        */
        Yaxt::Ptr m_yaxt;

        //!
        /*!
          Pointer to a Grid class object, which is initialized by the CCCC class constructor.
        */
        Grid::Ptr m_grid;

        //!
        /*!
          Pointer to a MPI class object, which is initialized by the CCCC class constructor.
        */
        MPI::Ptr  m_mpi;

    private:
        MPI_Comm m_intercomm;
        // map of fields
        std::map<int,std::vector<FieldType>> m_fields;
        // map of variables
        std::map<int,std::vector<VariableBase *>> m_variables;
        // map of commands
        std::map<int,std::vector<CmdType>> m_cmds;

        void exchange_variable_mpi_comp(int id, bool cond);
        void exchange_field_mpi_comp(int id, bool cond);
        void exchange_k2m_comp(int id);
        void exchange_m2k_comp(int id);
        void execute_cmds(int id);

  };
}
