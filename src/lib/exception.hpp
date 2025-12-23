#pragma once

#include <unordered_map>
#include "rainbow.hpp"

enum class YALLErrorTypes {
    TypeError,
    ValueError,
    VarError,
    LabelError,
    ModuleError,
    InstructionError,
    TokenError,
    NameError,
    RuntimeError,
};

static unordered_map<YALLErrorTypes, int> error_codes = {
    { YALLErrorTypes::TypeError, 100 },
    { YALLErrorTypes::ValueError, 101 },
    { YALLErrorTypes::VarError, 102 },
    { YALLErrorTypes::LabelError, 103 },
    { YALLErrorTypes::ModuleError, 104 },
    { YALLErrorTypes::InstructionError, 105 },
    { YALLErrorTypes::TokenError, 106 },
    { YALLErrorTypes::NameError, 107 },
    { YALLErrorTypes::RuntimeError, 108 }
};

static unordered_map<YALLErrorTypes, string> error_types = {
    { YALLErrorTypes::TypeError, "TypeError" },
    { YALLErrorTypes::ValueError, "ValueError" },
    { YALLErrorTypes::VarError, "VarError" },
    { YALLErrorTypes::LabelError, "LabelError" },
    { YALLErrorTypes::ModuleError, "ModuleError" },
    { YALLErrorTypes::InstructionError, "InstructionError" },
    { YALLErrorTypes::TokenError, "TokenError" },
    { YALLErrorTypes::NameError, "NameError" },
    { YALLErrorTypes::RuntimeError, "RuntimeError" }
};

class YALLException {
private:
    size_t line = 1;

public:
    void set_line(const int& line) { this->line = line; }

    void error(const string& message, const YALLErrorTypes& error_type, const int& current_line = 0) {
        cout << yall_rainbow.apply({ "red", "bold" }, error_types.at(error_type) + ": ") << message << " "
            << yall_rainbow.apply({ "yellow" }, (string("[line: ") + to_string(current_line == 0 ? this->line : current_line) + "]")) << endl;
        exit(error_codes.at(error_type));
    }

    void warn(const string& message, const int& current_line = 0) { 
        cout << yall_rainbow.apply({ "yellow", "bold" }, "Warning: ") << message << " "
            << yall_rainbow.apply({ "yellow" }, (string("[line: ") + to_string(current_line == 0 ? this->line : current_line) + "]")) << endl; 
    }
};

extern YALLException yall_exception;