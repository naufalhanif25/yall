#pragma once

#include "validator.hpp"
#include "modder.hpp"

enum class YALLVariableVisibility {
    Public,
    Private
};

class YALLGlobal {
public:
    struct YALLInstructionKey {
        string module;
        string instruction;
        vector<string> args;

        bool operator==(const YALLInstructionKey& other) const { 
            return module == other.module && instruction == other.instruction && args == other.args; 
        }
    };

    struct YALLInstructionKeyHash {
        size_t operator()(const YALLInstructionKey& key) const {
            size_t hashed = hash<string>{}(key.module);
            hashed ^= hash<string>{}(key.instruction) + 0x9e3779b9 + (hashed << 6) + (hashed >> 2);
            for (const auto& arg : key.args) hashed ^= hash<string>{}(arg) + 0x9e3779b9 + (hashed << 6) + (hashed >> 2);
            return hashed;
        }
    };

    struct YALLParsedInstruction {
        string name;
        pair<string, string> parts;
        int line;
        vector<string> args;
        vector<YALLParsedInstruction> children;

        void set_name(const string& name) { this->name = name; }
        void set_line(const int& line) { this->line = line; }
        void set_parts(const string& first, const string& second) {
            this->parts.first = first;
            this->parts.second = second;
        }
        void push_arg(const string& arg) { this->args.push_back(arg); }
        void push_child(const YALLParsedInstruction& children) { this->children.push_back(children); }
    };

    struct YALLVariable {
        string value;
        string label;
        YALLVariableVisibility visibility;
    };

    struct YALLTokens {
        string label;
        int line;
        vector<YALLParsedInstruction> instructions;

        bool empty() { return this->label.empty() || this->instructions.empty(); }
        void clear() {
            this->label.clear();
            this->instructions.clear();
        }
        void push_instruction(const YALLParsedInstruction& instruction) { this->instructions.push_back(instruction); }
        void set_label(const string& label) { this->label = label; }
        void set_line(const int& line) { this->line = line; }
        void set_instructions(const vector<YALLParsedInstruction>& instructions) { this->instructions = instructions; }
    };

protected:
    vector<YALLTokens> ast;
    string current_label;
    unordered_map<string, YALLVariable> variables;
    unordered_map<string, vector<YALLParsedInstruction>> label_index;

public:
    YALLGlobal() {
        this->ast = {};
        this->current_label = "main";
        this->variables = {};
        this->label_index = {};
    }

    void set_global_label(const string& label) { this->current_label = label; }

    void set_ast(const vector<YALLTokens>& tokens) { 
        this->ast = tokens; 
        label_index.clear();
        for (const auto& token : tokens) label_index[token.label] = token.instructions;
    }
    vector<YALLTokens> get_ast() { return this->ast; }

    vector<YALLParsedInstruction> get_instructions(const string& name) {
        auto it = label_index.find(name);

        if (it == label_index.end()) {
            if (name == "main") {
                string message = "The 'main' label was not found. Every program must define a 'main:' entry point";
                yall_exception.error(message, YALLErrorTypes::LabelError);
            }
            else {
                string message = "Label '" + name + "' not found";
                yall_exception.error(message, YALLErrorTypes::LabelError);
            }
            __builtin_unreachable();
        }
        auto result = it->second;

        if (result.empty()) {
            string message = "Instructions not found in module '" + name + "'";
            yall_exception.error(message, YALLErrorTypes::LabelError);
        }
        return result;
    }

    void store(const string& name, const string& value) {
        if (!yall_validator.is_name_valid(name)) {
            string message = "Invalid variable name: '" + name + "'. Variable names must follow naming rules";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        auto it = this->variables.find(name);

        if (it != variables.end()) {
            YALLVariable& current_variable = this->variables[name];

            if (current_variable.label != this->current_label && current_variable.visibility == YALLVariableVisibility::Private) {
                string message = "Private variable '" + name + "' cannot be accessed from outside its label";
                yall_exception.error(message, YALLErrorTypes::NameError);
            }
            else current_variable.value = value;
        }
        else this->variables[name] = { value, this->current_label, YALLVariableVisibility::Public };
    }

    string load(const string& name) {
        if (!yall_validator.is_name_valid(name)) {
            string message = "Invalid variable name: '" + name + "'. Variable names must follow naming rules";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        auto it = this->variables.find(name);

        if (it != variables.end()) {
            YALLVariable current_variable = this->variables[name];

            if (current_variable.label != this->current_label && current_variable.visibility == YALLVariableVisibility::Private) {
                string message = "Private variable '" + name + "' cannot be accessed from outside its label";
                yall_exception.error(message, YALLErrorTypes::NameError);
            }
            else return it->second.value;
        }
        string message = "Variable '" + name + "' was not found";
        yall_exception.error(message, YALLErrorTypes::VarError);

        __builtin_unreachable();
    }

    void set_visibility(const string& name, const YALLVariableVisibility& visibility) {
        if (!yall_validator.is_name_valid(name)) {
            string message = "Invalid variable name: '" + name + "'. Variable names must follow naming rules";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        auto it = this->variables.find(name);

        if (it != variables.end()) it->second.visibility = visibility;
        else {
            string message = "Variable '" + name + "' was not found";
            yall_exception.error(message, YALLErrorTypes::VarError);
        }
    }
};

extern YALLGlobal yall_global;