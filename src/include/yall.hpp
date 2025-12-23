#pragma once

#include "parser.hpp"

class YALL : protected YALLParser {
private:
    YALLTokens instructions_cache;

protected:
    string read(const string& path) {
        ifstream file(path, ios::in);
        if (!file) return "";

        ostringstream oss;
        oss << file.rdbuf();

        return oss.str();
    }

public:
    YALL() {
        this->instructions_cache = {};
    }

    void run(const string& path) {
        string data = this->read(path), label_name = "main";
        this->set_ast(this->parse(data));

        try {
            while (true) {
                if (this->instructions_cache.empty() || this->instructions_cache.label != label_name) {
                    yall_global.set_global_label(label_name);
                    this->instructions_cache.set_label(label_name);
                    this->instructions_cache.set_instructions(this->get_instructions(label_name));
                }
                bool jumped = false;

                for (const auto& instruction : this->instructions_cache.instructions) {
                    yall_exception.set_line(instruction.line);
                    const string output = this->eval_instruction(instruction, label_name);
                    
                    if (yall_validator.is_goto(output)) {
                        label_name = yall_modder.get_goto_label(output);
                        jumped = true;
                        break;
                    }
                }
                if (!jumped) break;
            }
        }
        catch (...) { yall_exception.error("A runtime error occurred while executing the program", YALLErrorTypes::RuntimeError); }
    }
};

YALL yall;