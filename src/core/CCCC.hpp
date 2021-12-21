#include <vector>
#include <map>

#include "CCCC/yaxt/Yaxt.hpp"
#include "CCCC/grid/Grid.hpp"
#include "CCCC/mpi/CMPI.hpp"

#define CCCC_CMD_EXIT -1
#define CCCC_K2M -2
#define CCCC_M2K -3

namespace DKRZ {

  typedef void (*fPtr)();

  struct FieldType {
    double *data; // pointer to the data
    Xt_redist *redist; // pointer to the redistributions
    bool m2k; // define the direction of the exchange (m2k or k2m)
  };

  struct VariableType {
    double *data; // pointer to the data
    int count; // define size
    bool m2k; // define the direction of the exchange (m2k or k2m)
  };

  struct CmdType {
    fPtr cmd; // pointer to function to execute
    int cmd_id;
  };

  class CCCC {
    public:
    	CCCC(MPI_Comm glob, int nprocs_kernel, std::string backend_name);
    	~CCCC();
    	int intercomm_create(int nmodel, int nprocs);
        void set_redist(int nlv);
        void add_field(double *data, int nlv, int nmodel, bool m2k);
        void add_variable(double *data, int count, int nmodel, bool m2k);
        void add_command(void (*Func_ptr) (), int nmodel, int cmd_id);
        void exchange_m2k(int nmodel);
        void exchange_k2m(int nmodel);
        void start_concurrent(int nmodel);
        void stop_concurrent(int nmodel);
        void execute(int nmodel, int cmd_id);
        bool has_kernel_role();

    protected:
        Yaxt::Ptr m_yaxt;
        Grid::Ptr m_grid;
        MPI::Ptr  m_mpi;

    private:
        bool m_kernel_role;
        std::vector<int> m_nprocs;
        // map of all the redistribution
        std::map<int,YaxtRedist> m_redist;
        // map of vectors to exchange on every stream
        std::map<int,std::vector<FieldType>> m_fields;
        // map of variables to exchange on every stream
        std::map<int,std::vector<VariableType>> m_variables;
        // map of vector to execute commands on every stream
        std::map<int,std::vector<CmdType>> m_cmds;

        // Functions
        void remote_loop(int nmodel);
        void exchange_m2k_impl(int nmodel);
        void exchange_k2m_impl(int nmodel);
        void send_data(int nmodel, bool cond);
        void recv_data(int nmodel, bool cond);
        void exchange_field(int nmodel, bool cond);
  };
}
