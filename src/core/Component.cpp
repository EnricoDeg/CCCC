#include <iostream>

#include "CCCC/core/Component.hpp"

namespace DKRZ {

    Component::Component(MPI_Comm intercomm, Yaxt::Ptr yaxtp, MPI::Ptr mpip) {
        m_intercomm = intercomm;
        m_yaxt = yaxtp;
        m_mpi = mpip;
    }

    Component::~Component() {

    }

    void Component::add_field_impl(double *data, Xt_redist * rds, int size, int id, bool m2k) {
        FieldType f;
        f.data = data;
        f.redist = rds;
        f.size = size;
        f.m2k = m2k;
        m_fields[id].push_back(f);
    }

    void Component::exchange_field_yaxt_impl(int id, bool cond) {
        for (auto field : m_fields[id]) {
            if (field.m2k == cond) {
                m_yaxt->exchange(*(field.redist), field.data);
            }
        }
    }

    void Component::exchange_field_mpi_impl(int id, bool cond, bool sender) {
        for (auto field : m_fields[id]) {
            if (field.m2k == cond) {
                m_mpi->exchange(field.data, field.size, sender, m_intercomm);
            }
        }
    }

    void Component::add_variable_impl(double *data, int count, int id, bool m2k) {
        VariableType f;
        f.data = data;
        f.count = count;
        f.m2k = m2k;
        m_variables[id].push_back(f);
    }

    void Component::exchange_variable_mpi_impl(int id, bool cond, bool sender) {
        for (auto variable : m_variables[id]) {
            if (variable.m2k == cond) {
                m_mpi->exchange(variable.data, variable.count, sender, m_intercomm);
            }
        }
    }

    void Component::add_command_impl(void (*Func_ptr) (), int cmd_id) {
        CmdType c;
        c.cmd = Func_ptr;
        m_cmds[cmd_id].push_back(c);
    }

    void Component::exchange_variable_mpi_comp(int id, bool cond) {
        bool sender;

        if (cond) { // m2k
            sender = false; // kernel recv data
        } else { //k2m
            sender = true; // kernel send data
        }

        exchange_variable_mpi_impl(id, cond, sender);
    }

    void Component::exchange_field_mpi_comp(int id, bool cond) {
        bool sender;

        if (cond) { // m2k
                sender = false; // kernel recv data
        } else { //k2m
                sender = true; // kernel send data
        }

        exchange_field_mpi_impl(id, cond, sender);
    }

    void Component::exchange_k2m_comp(int id) {
        exchange_variable_mpi_comp(id, false);
        exchange_field_mpi_comp(id, false);
    }

    void Component::exchange_m2k_comp(int id) {
        exchange_variable_mpi_comp(id, true);
        exchange_field_mpi_comp(id, true);
    }

    void Component::execute_cmds(int id) {
        for (auto stream_cmds : m_cmds[id]) {
            stream_cmds.cmd(); // execute command with received ID
        }
    }

    void Component::remote_loop_impl() {
        int i_cmd = m_mpi->get_cmd(m_intercomm);
        int id;
        while (i_cmd != CCCC_CMD_EXIT) {
            switch(i_cmd) {
                case(CCCC_K2M):
                    id = m_mpi->get_cmd(m_intercomm);
                    exchange_k2m_comp(id);
                    break;
                case(CCCC_M2K):
                    id = m_mpi->get_cmd(m_intercomm);
                    exchange_m2k_comp(id);
                    break;
                default:
                    execute_cmds(i_cmd);
                    break;
            }
            i_cmd = m_mpi->get_cmd(m_intercomm);
        }
    }

    void Component::send_cmd(int cmd_id) {
        m_mpi->send_cmd(cmd_id, m_intercomm);
    }

}
