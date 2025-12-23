#pragma once

#include "modules.hpp"

class YALLExecutor : protected YALLModules {
private:
    struct YALLInstructionName {
        string mod_name;
        string instruct_name;

        void set(const string& mod_name, const string& instruct_name) {
            this->mod_name = mod_name;
            this->instruct_name = instruct_name;
        }
    };
    unordered_map<string, YALLInstructionName> instruction_name_cache;
    unordered_map<string, unordered_map<YALLInstructionKey, string, YALLInstructionKeyHash>> instruction_output_cache;

    string label_name;

public:
    YALLExecutor() {
        this->instruction_name_cache = {};
        this->label_name = "main";
    }

    string execute(const string& name, const pair<string, string>& parts, vector<string>& args, const string& label) {
        if (this->label_name != label) {
            instruction_name_cache.erase(this->label_name);
            instruction_output_cache.clear();
        }
        YALLInstructionName instruction_data;

        auto name_it = instruction_name_cache.find(name);
        if (name_it != instruction_name_cache.end()) instruction_data = name_it->second;
        else {
            instruction_data.set(parts.first, parts.second);
            instruction_name_cache[name] = instruction_data;
        }
        
        if (!yall_validator.is_name_valid(instruction_data.mod_name)) {
            string message = "Invalid module name '" + instruction_data.mod_name + "': naming format is not recognized";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        if (!yall_validator.is_name_valid(instruction_data.instruct_name)) {
            string message = "Invalid instruction name '" + instruction_data.instruct_name + "': naming format is not recognized";
            yall_exception.error(message, YALLErrorTypes::NameError);
        }
        YALLInstructionData instruction = yall_modules.get_instruction(instruction_data.mod_name, instruction_data.instruct_name);

        if (instruction.is_pure) {
            auto& current_output_cache = instruction_output_cache[label];
            YALLInstructionKey key = { instruction_data.mod_name, instruction_data.instruct_name, args };

            auto it = current_output_cache.find(key);
            if (it != current_output_cache.end()) return it->second;

            string result = instruction.callback(args);
            current_output_cache.emplace(key, result);
            
            return result;
        }

        return instruction.callback(args);
    }
};