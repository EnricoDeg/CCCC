#include <vector>
#include <map>

#include "CCCC/grid/Grid.hpp"
#include "CCCC/core/Component.hpp"

#define CCCC_CMD_EXIT -1
#define CCCC_K2M -2
#define CCCC_M2K -3

namespace DKRZ {

  //!  library main class
  /*!
    It is the interface between CCCC library internals and the external
    model using the library.
  */
  class CCCC {
    public:
        //!
        /*!
          Initialize MPI and YAXT if needed. Then, initialize CCCC library
          defining kernel or model role for each process and creating the 
          grid backend.

          \param glob communicator including all processes that initialized
                 the library
          \param nprocs_kernel number of processes within the kernel
          \param backend_name input string to select and initialize a grid backend
        */
    	CCCC(MPI_Comm glob, int nprocs_kernel, std::string backend_name);

        //!
        /*!
          Finalize CCCC library. Then, finalize MPI and YAXT if needed.
        */
    	~CCCC();

        //!
        /*!
          Create an intercommunicator between kernel communicator and model
          communicator.
          \param nmodel model ID
          \nprocs number of processes within the model
        */
    	void intercomm_create(int nmodel, int nprocs);

        void grid_subdomain_start(int i, int j);

        void grid_subdomain_end(int i, int j);

        void grid_subdomain_ext(int i, int j);

        void grid_subdomain_off(int i, int j);

        void grid_domain_ext(int i, int j);

        //!
        /*!
          Create a YAXT redistribution for a given model domain decomposition 
          based on the number of vertical levels. It requires that the model and the
          kernel domain decompositions are already defined.

          The following steps are performed within this function call:

          1. Create an index list

          2. Define the individual process domain decomposition offsets

          3. Generate the YAXT redistribution between the model and the kernel in 
             both directions

          \param nmodel model ID
          \param nlv number of vertical levels 
        */
        void set_redist(int nmodel, int nlv);

        //!
        /*!
          Add a field to the field list of a selected model.
          \param data pointer to the field allocated data
          \param nlv number of vertical levels
          \param nmodel model ID
          \param id exchange ID
          \param m2k exchange direction
        */
        void add_field(double *data, int nlv, int nmodel, int id, bool m2k);

        //!
        /*!
          Add a variable to the variable list of a selected model.
          \param data pointer to the variable allocated data
          \param count variable allocated size
          \param nmodel model ID
          \param id exchange ID
          \param m2k exchange direction
        */
        template <typename T>
        void add_variable(T *data, int count, int nmodel, int id, bool m2k);

        //!
        /*!
          Add a command to the command list of a selected model.
          \param Func_ptr pointer to the function (command) to be executed
                          by the model
          \param nmodel model ID
          \param cmd_id command ID
        */
        void add_command(void (*Func_ptr) (), int nmodel, int cmd_id);

        //!
        /*!
          Send the fields and the variables from the kernel to a given model
          with a selected exchange ID.
          \param nmodel model ID
          \param id exchange ID
        */
        void exchange_m2k(int nmodel, int id);

        //!
        /*!
          Send the fields and the variables from a selected  model with a given
          exchange ID to the kernel.
          \param nmodel model ID
          \param id exchange ID
        */
        void exchange_k2m(int nmodel, int id);

        //!
        /*!
          Start the concurrent execution of a selected model. This implies that the
          initialization of the given model is completed and all the processes 
          within the model are sent in an infinite loop waiting for command to execute.
          \param nmodel model ID
        */
        void start_concurrent(int nmodel);

        //!
        /*!
          Terminate the concurrent execution of a selected model. This means that all
          the processes within the model exit from the infinite loop and return from the 
          start_concurrent() function call.
        */
        void stop_concurrent(int nmodel);

        //!
        /*!
          The kernel processes send a message to the selected model ID processes to 
          execute all the commands labeled with the selected command ID.
          \param nmodel model ID
          \param cmd_id command ID
        */
        void execute(int nmodel, int cmd_id);

        bool has_kernel_role();

        MPI_Comm local_comm();

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
        bool m_kernel_role;
        std::vector<int> m_nprocs;
        // map of all the redistribution
        std::map<int,YaxtRedist> m_redist;
        // map of components obj
        std::map<int,Component::Ptr> m_components;

        // Functions
        void remote_loop(int nmodel);
        void full_exchange(int nmodel, int id, bool cond);
  };
}
