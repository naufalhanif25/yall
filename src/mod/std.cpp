#include "../include/modules.hpp"

void init_std() {
    YALLModule* std_mod = yall_modules.new_module("std");

    std_mod->add_instruction("cout", false, [](const vector<string>& args) {
        cout << yall_modder.eval_unescape(args.size() > 1 ? yall_modder.join_vector(args, " ") : args[0]) << endl;
        return NULL_STR;
    });

    std_mod->add_instruction("cerr", false, [](const vector<string>& args) {
        cerr << yall_modder.eval_unescape(args.size() > 1 ? yall_modder.join_vector(args, " ") : args[0]) << endl;
        return NULL_STR;
    });

    std_mod->add_instruction("cin", false, [](const vector<string>& args) {
        string answer;
        cout << yall_modder.eval_unescape(args.size() > 1 ? yall_modder.join_vector(args, " ") : args[0]) << endl;
        getline(cin, answer);
        return yall_modder.quote_string(answer);
    });
    
    std_mod->add_instruction("exit", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'exit'. Expected exactly 1 argument (code)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0];
        if (!yall_validator.is_number(input)) {
            string message = "Argument for 'exit' must be a number. Invalid value: '" + input + "'";
            yall_exception.error(message, YALLErrorTypes::TypeError);
        }
        exit(yall_modder.stoi(input));
        return NULL_STR;
    });

    std_mod->add_instruction("store", false, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'store'. Expected exactly 2 arguments (name and value)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        yall_global.store(args[0], args[1]);
        return NULL_STR;
    });

    std_mod->add_instruction("pub", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'pub'. Expected exactly 1 argument (name)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        yall_global.set_visibility(args[0], YALLVariableVisibility::Public);
        return NULL_STR;
    });

    std_mod->add_instruction("priv", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'priv'. Expected exactly 1 argument (name)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        yall_global.set_visibility(args[0], YALLVariableVisibility::Private);
        return NULL_STR;
    });

    std_mod->add_instruction("load", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'load'. Expected exactly 1 argument (name)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string result = yall_global.load(args[0]);
        if (!result.empty()) return result;
        return NULL_STR;
    });

    std_mod->add_instruction("goto", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'goto'. Expected exactly 1 argument (label name)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string label_name = args[0];
        if (!yall_validator.is_name_valid(label_name)) {
            string message = "Invalid label name: '" + label_name + "'. Label names must follow naming rules";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        return yall_modder.to_goto(label_name);
    });

    std_mod->add_instruction("goif", false, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'goif'. Expected exactly 1 argument (label name and condition)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string label_name = args[1], condition = args[0];
        if (!yall_validator.is_name_valid(label_name)) {
            string message = "Invalid label name: '" + label_name + "'. Label names must follow naming rules";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        if (!yall_validator.is_bool(condition)) {
            string message = "Condition argument for 'goif' must be a boolean ('true' or 'false'), got '" + condition + "'";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        if (condition == TRUE_STR) return yall_modder.to_goto(label_name);
        return NULL_STR;
    });
}
