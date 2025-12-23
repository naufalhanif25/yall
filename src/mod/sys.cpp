#include <filesystem>
#include <sys/utsname.h>

#include "../include/modules.hpp"

namespace fs = std::filesystem;

void init_sys() {
    YALLModule* sys_mod = yall_modules.new_module("sys");

    sys_mod->add_instruction("run", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'run'. Expected exactly 1 argument (command)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string cmd = args[0];
        cmd = yall_validator.is_string(cmd) ? yall_modder.trim_quotes(cmd) : cmd;
        system(cmd.c_str());
        return NULL_STR;
    });

    sys_mod->add_instruction("name", false, [](const vector<string>& args) {
        if (!args.empty()) {
            string message = "Invalid number of arguments for 'name'. This instruction does not accept any arguments";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        struct utsname info;
        return string(info.sysname);
    });

    sys_mod->add_instruction("exist", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'exist'. Expected exactly 1 argument (path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return fs::exists(args[0]) ? TRUE_STR : FALSE_STR;
    });

    sys_mod->add_instruction("isfile", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'isfile'. Expected exactly 1 argument (file path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return fs::is_regular_file(args[0]) ? TRUE_STR : FALSE_STR;
    });

    sys_mod->add_instruction("isdir", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'isdir'. Expected exactly 1 argument (path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return fs::is_directory(args[0]) ? TRUE_STR : FALSE_STR;
    });

    sys_mod->add_instruction("mkdir", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'mkdir'. Expected exactly 1 argument (path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        fs::create_directory(args[0]);
        return NULL_STR;
    });

    sys_mod->add_instruction("rm", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'rm'. Expected exactly 1 argument (path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        fs::remove(args[0]);
        return NULL_STR;
    });

    sys_mod->add_instruction("rmrf", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'rmrf'. Expected exactly 1 argument (path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        fs::remove_all(args[0]);
        return NULL_STR;
    });
}