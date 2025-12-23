#include "../include/modules.hpp"

void init_str() {
    YALLModule* str_mod = yall_modules.new_module("str");

    str_mod->add_instruction("cat", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "Invalid number of arguments for 'cat'. Expected at least 1 argument";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        vector<string> processed;
        for (const auto& arg : args) processed.push_back(yall_validator.is_string(arg) ? yall_modder.trim_quotes(arg) : arg);
        string result;
        for (const auto& str : processed) result += str;
        return yall_modder.quote_string(result);
    });

    str_mod->add_instruction("sub", true, [](const vector<string>& args) {
        if (args.size() != 3) {
            string message = "Invalid number of arguments for 'sub'. Expected exactly 3 arguments (input, start, end)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0], start = args[1], end = args[2];
        if ((yall_validator.is_string(start) || !yall_validator.is_number(start)) || (yall_validator.is_string(end) || !yall_validator.is_number(end))) {
            string message = "Start and end indexes must be numbers";
            yall_exception.error(message, YALLErrorTypes::TypeError);
        }
        int start_index = yall_modder.stoi(start), end_index = yall_modder.stoi(end);
        input = yall_validator.is_string(input) ? yall_modder.trim_quotes(input) : input;
        if (start_index < 0 || end_index > input.size() || start_index > end_index) return string("");
        return yall_modder.quote_string(input.substr(start_index, end_index - start_index));
    });

    str_mod->add_instruction("len", true, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'len'. Expected exactly 1 argument (input)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0], result = yall_validator.is_string(input) ? yall_modder.trim_quotes(input) : input;
        return to_string(result.size());
    });

    str_mod->add_instruction("fmt", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "Invalid number of arguments for 'fmt'. Expected at least 1 argument";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string message = args[0];
        vector<string> inputs(args.begin() + 1, args.end());
        
        if (yall_validator.is_string(message)) message = yall_modder.trim_quotes(message);
        for (size_t idx = 0, index = 0; idx < message.size(); idx++) {
            size_t pos = message.find("{}", index);
            if (pos == string::npos) break;
            string current = inputs[index], replacement = index < inputs.size() ? (yall_validator.is_string(current) ? yall_modder.trim_quotes(current) : current) : "{}";
            message.replace(pos, 2, replacement);
            index++;
        }
        return yall_modder.quote_string(message);
    });
}
