#pragma once

#include "lexer.hpp"

using InstructionCallback = function<string(const vector<string>&)>;

class YALLModule : protected YALLLexer {
protected:
    struct YALLInstructionData {
        bool is_pure;
        InstructionCallback callback;
    };

    string name;
    unordered_map<string, YALLInstructionData> instructions;

public:
    YALLModule() : name("std") {}
    YALLModule(const string& name) {
        this->name = name;
        this->instructions = {};
    }

    YALLInstructionData get_module_instruction(const string& name) {
        auto it = instructions.find(name);
        if (it != instructions.end()) return it->second;
        string message = "Instruction '" + name + "' not found in module '" + this->name + "'";
        yall_exception.error(message, YALLErrorTypes::InstructionError);
        
        __builtin_unreachable();
    }

    void add_instruction(const string& name, const bool& is_pure, InstructionCallback callback) {
        pair<string, YALLInstructionData> instruction = { name, { is_pure, callback } };
        this->instructions.insert({ instruction.first, instruction.second });
    }

    string execute_instruction(const string& name, const vector<string>& args) {
        auto it = instructions.find(name);
        if (it != instructions.end()) return it->second.callback(args);
        string message = "Instruction '" + name + "' not found in module '" + this->name + "'";
        yall_exception.error(message, YALLErrorTypes::InstructionError);

        __builtin_unreachable();
    }
};