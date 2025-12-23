#pragma once

#include "module.hpp"

static const string NULL_STR = "null";
static const string TRUE_STR = "true";
static const string FALSE_STR = "false";

class YALLModules : protected YALLModule {
private:
    unordered_map<string, YALLModule*> modules;

public:
    YALLModule* new_module(const string& name) {
        YALLModule* mod = new YALLModule(name);
        modules[name] = mod;
        return mod;
    }

    YALLInstructionData get_instruction(const string& mod_name, const string& instruction_name) {
        auto it = modules.find(mod_name);
        if (it != modules.end()) return it->second->get_module_instruction(instruction_name);
        else {
            string message = "Module '" + mod_name + "' not found";
            yall_exception.error(message, YALLErrorTypes::ModuleError);
        }
        __builtin_unreachable();
    }

    string execute_instruction(const string& mod_name, const string& instruction_name, const vector<string>& args) {
        auto it = modules.find(mod_name);
        if (it != modules.end()) return it->second->execute_instruction(instruction_name, args);
        else {
            string message = "Module '" + mod_name + "' not found";
            yall_exception.error(message, YALLErrorTypes::ModuleError);
        }
        __builtin_unreachable();
    }
};

extern YALLModules yall_modules;