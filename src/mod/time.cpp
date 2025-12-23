#include <chrono>

#include "../include/modules.hpp"

using namespace std::chrono;

long long parse_ns(const string& value) {
    if (!yall_validator.is_number(value)) {
        string message = "Time value must be a number, got '" + value + "'";
        yall_exception.error(message, YALLErrorTypes::TypeError);
    }
    return yall_modder.stoll(value);
}

static InstructionCallback make_time_convert(const string& name, long long divisor, const string& unit_name) {
    return [name, divisor, unit_name](const vector<string>& args) -> string {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for '" + name + "'. Expected exactly 1 argument (nanoseconds)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        if (!yall_validator.is_number(args[0])) {
            string message = "Argument for '" + name + "' must be a number representing nanoseconds";
            yall_exception.error(message, YALLErrorTypes::TypeError);
        }
        return to_string(yall_modder.stoll(args[0]) / divisor);
    };
}

void init_time() {
    YALLModule* time_mod = yall_modules.new_module("time");

    time_mod->add_instruction("now", false, [](const vector<string>& args) {
        if (!args.empty()) {
            string message = "Invalid number of arguments for 'now'. This instruction does not accept any arguments";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return to_string(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
    });

    time_mod->add_instruction("diff", true, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'diff'. Expected exactly 2 arguments (start, end)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        long long start = parse_ns(args[0]), end = parse_ns(args[1]);
        if (end < start) {
            string message = "End time must be greater than or equal to start time";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        return to_string(end - start);
    });

    time_mod->add_instruction("micro", true, make_time_convert("micro", 1'000LL, "microsecond"));
    time_mod->add_instruction("mili", true, make_time_convert("mili", 1'000'000LL, "millisecond"));
    time_mod->add_instruction("sec", true, make_time_convert("sec", 1'000'000'000LL, "second"));
    time_mod->add_instruction("min", true, make_time_convert("min", 60LL * 1'000'000'000LL, "minute"));
    time_mod->add_instruction("hour", true, make_time_convert("hour", 60LL * 60LL * 1'000'000'000LL, "hour"));
}