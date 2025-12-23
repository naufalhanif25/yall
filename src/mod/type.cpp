#include "../include/modules.hpp"

static void add_casting_instruction(YALLModule* type_mod, const string& instruct_name, const bool& is_pure, const string& type_name, function<bool(const string&)> comparator) {
    type_mod->add_instruction(instruct_name, is_pure, [=](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for '" + instruct_name + "'. Expected exactly 1 argument (value)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0], output = yall_validator.is_string(input) ? yall_modder.trim_quotes(input) : input;
        
        if (!comparator(output)) {
            string message =
                "Invalid value for '" + instruct_name + "'. Expected a " + type_name + ", got '" + input + "'";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return output;
    });
}

void init_type() {
    YALLModule* type_mod = yall_modules.new_module("type");

    type_mod->add_instruction("of", true, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'of'. Expected exactly 1 argument (value)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0], type;
        if (yall_validator.is_string(input)) type = "string";
        else if (yall_validator.is_null(input)) type = "null";
        else if (yall_validator.is_number(input)) type = "number";
        else if (yall_validator.is_bool(input)) type = "bool";
        else if (yall_validator.is_instruction(input)) type = "instruct";
        else type = "unknown";
        return yall_modder.quote_string(type);
    });

    type_mod->add_instruction("str", true, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'str'. Expected exactly 1 argument (value)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0];
        return yall_validator.is_string(input) ? input : yall_modder.quote_string(input);
    });

    add_casting_instruction(type_mod, "num", true, "number", [](string a){ return yall_validator.is_number(a); });
    add_casting_instruction(type_mod, "bool", true, "boolean", [](string a){ return yall_validator.is_bool(a); });
}
