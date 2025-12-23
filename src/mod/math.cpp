#include <cmath>
#include <algorithm>
#include <climits>

#include "../include/modules.hpp"

static void add_compare_instruction(YALLModule* math_mod, const string& name, const bool& is_pure, function<bool(double, double)> comparator) {
    math_mod->add_instruction(name, is_pure, [=](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for '" + name + "'. Expected exactly 2 numbers";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        for (const string& arg : args) {
            if (!yall_validator.is_number(arg)) {
                string message = "All arguments for '" + name + "' must be numbers. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
        }
        return comparator(yall_modder.stod(args[0]), yall_modder.stod(args[1])) ? TRUE_STR : FALSE_STR;
    });
}

void init_math() {
    YALLModule* math_mod = yall_modules.new_module("math");

    math_mod->add_instruction("add", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'add'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        double result = 0;
        for (const string& arg : args) {
            if (!yall_validator.is_number(arg)) {
                string message = "All arguments for 'add' must be numbers. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            result += yall_modder.stod(arg);
        }
        return yall_modder.trim_trailing_zeros(to_string(result));
    });

    math_mod->add_instruction("sub", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'sub'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        double result = yall_modder.stod(args[0]);
        for (size_t index = 1; index < args.size(); index++) {
            if (!yall_validator.is_number(args[index])) {
                string message = "All arguments for 'sub' must be numbers. Invalid value: '" + args[index] + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            result -= yall_modder.stod(args[index]);
        }
        return yall_modder.trim_trailing_zeros(to_string(result));
    });

    math_mod->add_instruction("mult", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'mult'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        double result = 1;
        for (const string& arg : args) {
            if (!yall_validator.is_number(arg)) {
                string message = "All arguments for 'mult' must be numbers. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            result *= yall_modder.stod(arg);
        }
        return yall_modder.trim_trailing_zeros(to_string(result));
    });

    math_mod->add_instruction("div", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'div'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        double result = yall_modder.stod(args[0]);
        for (size_t index = 1; index < args.size(); index++) {
            if (!yall_validator.is_number(args[index])) {
                string message = "All arguments for 'div' must be numbers. Invalid value: '" + args[index] + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            double divisor = yall_modder.stod(args[index]);
            if (divisor == 0) {
                string message = "Division by zero in 'div'";
                yall_exception.error(message, YALLErrorTypes::ValueError);
            }
            result /= divisor;
        }
        return yall_modder.trim_trailing_zeros(to_string(result));
    });

    math_mod->add_instruction("mod", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'mod'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        int result = yall_modder.stoi(args[0]);
        for (size_t index = 1; index < args.size(); index++) {
            if (!yall_validator.is_number(args[index])) {
                string message = "All arguments for 'mod' must be numbers. Invalid value: '" + args[index] + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            int divisor = yall_modder.stoi(args[index]);
            if (divisor == 0) {
                string message = "Modulo by zero in 'mod'";
                yall_exception.error(message, YALLErrorTypes::ValueError);
            }
            result %= divisor;
        }
        return to_string(result);
    });

    math_mod->add_instruction("pow", true, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'pow'. Expected exactly 2 numbers (base, exponent)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        if (!yall_validator.is_number(args[0]) || !yall_validator.is_number(args[1])) {
            string message = "Both arguments for 'pow' must be numbers";
            yall_exception.error(message, YALLErrorTypes::TypeError);
        }
        return yall_modder.trim_trailing_zeros(to_string(pow(yall_modder.stod(args[0]), yall_modder.stod(args[1]))));
    });

    math_mod->add_instruction("max", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'max'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        double maximum = INT_MIN;
        for (const string& arg : args) {
            if (!yall_validator.is_number(arg)) {
                string message = "All arguments for 'max' must be numbers. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            maximum = max(maximum, yall_modder.stod(arg));
        }
        return yall_modder.trim_trailing_zeros(to_string(maximum));
    });

    math_mod->add_instruction("min", true, [](const vector<string>& args) {
        if (args.empty()) {
            string message = "No arguments provided for 'min'. Expected at least 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        double minimum = INT_MAX;
        for (const string& arg : args) {
            if (!yall_validator.is_number(arg)) {
                string message = "All arguments for 'min' must be numbers. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            minimum = min(minimum, yall_modder.stod(arg));
        }
        return yall_modder.trim_trailing_zeros(to_string(minimum));
    });

    math_mod->add_instruction("round", true, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'round'. Expected exactly 1 number";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0];
        if (!yall_validator.is_number(input)) {
            string message = "Argument for 'round' must be a number. Invalid value: '" + input + "'";
            yall_exception.error(message, YALLErrorTypes::TypeError);
        }
        return to_string(static_cast<int>(round(yall_modder.stod(input))));
    });

    math_mod->add_instruction("equ", true, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'equ'. Expected exactly 2 arguments";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return args[0] == args[1] ? TRUE_STR : FALSE_STR;
    });

    math_mod->add_instruction("notequ", true, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'notequ'. Expected exactly 2 arguments";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return args[0] != args[1] ? TRUE_STR : FALSE_STR;
    });

    math_mod->add_instruction("not", true, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'not'. Expected exactly 1 argument";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        string input = args[0];
        if (!yall_validator.is_bool(input)) {
            string message = "Argument for 'not' must be a boolean. Invalid value: '" + input + "'";
            yall_exception.error(message, YALLErrorTypes::TypeError);
        }
        return input == TRUE_STR ? FALSE_STR : TRUE_STR;
    });

    math_mod->add_instruction("and", true, [](const vector<string>& args) {
        if (args.size() < 2) {
            string message = "Invalid number of arguments for 'and'. Expected at least 2 arguments";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        for (const string& arg : args) {
            if (!yall_validator.is_bool(arg)) {
                string message = "All arguments for 'and' must be booleans. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            if (arg == FALSE_STR) return FALSE_STR;
        }
        return TRUE_STR;
    });

    math_mod->add_instruction("or", true, [](const vector<string>& args) {
        if (args.size() < 2) {
            string message = "Invalid number of arguments for 'or'. Expected at least 2 arguments";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        for (const string& arg : args) {
            if (!yall_validator.is_bool(arg)) {
                string message = "All arguments for 'or' must be booleans. Invalid value: '" + arg + "'";
                yall_exception.error(message, YALLErrorTypes::TypeError);
            }
            if (arg == TRUE_STR) return TRUE_STR;
        }
        return FALSE_STR;
    });

    add_compare_instruction(math_mod, "lt", true, [](double a, double b) { return a < b; });
    add_compare_instruction(math_mod, "gt", true, [](double a, double b) { return a > b; });
    add_compare_instruction(math_mod, "ltequ", true, [](double a, double b) { return a <= b; });
    add_compare_instruction(math_mod, "gtequ", true, [](double a, double b) { return a >= b; });
}
