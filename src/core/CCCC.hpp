#include <vector>
#include <map>

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
    int exchange_id; // define the exchange id
    bool m2k; // define the direction of the exchange (m2k or k2m)
  };

  struct VariableType {
    double *data; // pointer to the data
    int count; // define size
    int exchange_id; // define the exchange id
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
    	void intercomm_create(int nmodel, int nprocs);
        void grid_subdomain_start(int i, int j);
        void grid_subdomain_end(int i, int j);
        void grid_subdomain_ext(int i, int j);
        void grid_subdomain_off(int i, int j);
        void grid_domain_ext(int i, int j);
        void set_redist(int nmodel, int nlv);
        void add_field(double *data, int nlv, int nmodel, int id, bool m2k);
        void add_variable(double *data, int count, int nmodel, int id, bool m2k);
        void add_command(void (*Func_ptr) (), int nmodel, int cmd_id);
        void exchange_m2k(int nmodel, int id);
        void exchange_k2m(int nmodel, int id);
        void start_concurrent(int nmodel);
        void stop_concurrent(int nmodel);
        void execute(int nmodel, int cmd_id);
        bool has_kernel_role();
        MPI_Comm local_comm();

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
        void exchange_m2k_impl(int nmodel, int id);
        void exchange_k2m_impl(int nmodel, int id);
        void send_data(int nmodel, int id, bool cond);
        void recv_data(int nmodel, int id, bool cond);
        void exchange_field(int nmodel, int id, bool cond);
  };
}
