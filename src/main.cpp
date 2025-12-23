#include "include/cli.hpp"

void init_std();
void init_math();
void init_str();
void init_time();
void init_sys();
void init_type();
void init_set();
void init_fs();

void init_modules() {
    init_std();
    init_math();
    init_str();
    init_time();
    init_sys();
    init_type();
    init_set();
    init_fs();
}

int main(int argc, char* argv[]) {
    init_modules();
    
    yall_cli.handle(argc, argv);
    exit(0);
}