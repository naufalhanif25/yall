#include <fstream>

#include "../include/modules.hpp"

void init_fs() {
    YALLModule* fs_mod = yall_modules.new_module("fs");

    fs_mod->add_instruction("read", false, [](const vector<string>& args) {
        if (args.size() != 1) {
            string message = "Invalid number of arguments for 'read'. Expected exactly 1 argument (file path)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        ifstream file(args[0], ios::in | ios::binary);
        if (!file) return NULL_STR;
        file.seekg(0, ios::end);

        string content;
        content.resize(file.tellg());
        file.seekg(0, ios::beg);
        file.read(&content[0], content.size());

        return content;
    });

    fs_mod->add_instruction("write", false, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'write'. Expected exactly 2 argument (file path, content)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        ofstream file(args[0], ios::out | ios::binary);
        if (!file) return NULL_STR;

        string content = args[1];
        file.write(content.data(), content.size());

        return NULL_STR;
    });

    fs_mod->add_instruction("append", false, [](const vector<string>& args) {
        if (args.size() != 2) {
            string message = "Invalid number of arguments for 'append'. Expected exactly 2 argument (file path, content)";
            yall_exception.error(message, YALLErrorTypes::ValueError);
        }
        ofstream file(args[0], ios::out | ios::app | ios::binary);
        if (!file) return NULL_STR;

        string content = args[1];
        file.write(content.data(), content.size());

        return NULL_STR;
    });
}