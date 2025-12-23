#include "../include/modules.hpp"

void init_set() {
    YALLModule* set_mod = yall_modules.new_module("set");

    set_mod->add_instruction("new", true, [](const vector<string>& args) {
        return yall_modder.build_set(args);
    });

    set_mod->add_instruction("at", true, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'at'. Expected exactly 2 arguments (set, index)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string set_data = args[0];
        if (!yall_validator.is_set(set_data)) {
            string message = "First argument for 'at' must be a set. Invalid value: '" + set_data + "'";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string str_index = args[1];
        if (!yall_validator.is_number(str_index)) {
            string message = "Second argument for 'at' must be a number (index). Invalid value: '" + str_index + "'";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        vector<string> data = yall_modder.parse_set(set_data);
        int index = yall_modder.stoi(str_index);

        if (index < 0 || index >= static_cast<int>(data.size())) {
            string message = "Index out of range for 'at'. Index " + str_index + " is out of bounds (size: " + to_string(data.size()) + ")";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return data[index];
    });

    set_mod->add_instruction("push", true, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'push'. Expected exactly 2 arguments (set, value)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string set_data = args[0];

        if (!yall_validator.is_set(set_data)) {
            string message = "First argument for 'push' must be a set. Invalid value: '" + set_data + "'";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        vector<string> data = yall_modder.parse_set(set_data);
        data.push_back(args[1]);
        return yall_modder.build_set(data);
    });

    set_mod->add_instruction("pop", true, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'pop'. Expected exactly 1 arguments (set)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string set_data = args[0];
        
        if (!yall_validator.is_set(set_data)) {
            string message = "First argument for 'pop' must be a set. Invalid value: '" + set_data + "'";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        vector<string> data = yall_modder.parse_set(set_data);
        data.pop_back();
        return yall_modder.build_set(data);
    });
}