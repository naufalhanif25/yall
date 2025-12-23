#include "yall.hpp"

class YALLCLI {
private:
    struct YALLOption {
        vector<string> options;
        string description;
        function<void(const vector<string>& args, int* pos)> callback;
        vector<string> examples;
    };
    vector<YALLOption> option_list;

    void show_help() {
        cout << string(DESCRIPTION) << " " << yall_rainbow.apply({ "yellow" }, "(" + string(VERSION) + ")") << endl << endl;

        cout << yall_rainbow.apply({ "bold" }, "Usage: " + string(NAME) + string(" ")) << yall_rainbow.apply({ "cyan", "bold" }, "[option...] [argument...]") << endl << endl;
        cout << yall_rainbow.apply({ "bold" }, "Options:") << endl;

        for (const auto& option : this->option_list) {
            size_t length = option.options.size();

            cout << "  ";
            for (int index = 0; index < length; index++) {
                cout << yall_rainbow.apply({ "cyan" }, option.options[index]);
                if (index + 1 < length) cout << ", ";
            }
            cout << "\t\t" << option.description << endl;
            
            cout << "  " << "\t\t\t" << yall_rainbow.apply({ "bold" }, "Example: ");
            for (int index = 0; index < option.examples.size(); index++) {
                cout << yall_rainbow.apply({ "dim" }, string(NAME) + string(" ") + option.examples[index]);
                if (index + 1 < length) cout << ", ";
            }
            cout << endl;
        }
        
        cout << endl << yall_rainbow.apply({ "bold" }, "Learn more about HRON:") << "\t" << yall_rainbow.apply({ "yellow" }, "https://github.com/naufalhanif25/yall.git") << endl;
    }

public:
    YALLCLI() {
        this->option_list = {
            {
                {
                    { "-h", "--help" },
                    "Show help message",
                    [this](const vector<string>& args, int* pos){
                        this->show_help();
                        exit(0);
                    },
                    { "-h", "--help" }
                },
                {
                    { "-r", "--run" },
                    "Run YALL program code",
                    [this](const vector<string>& args, int* pos){
                        if (args.size() != 1) {
                            string message = "Invalid number of arguments for '-r' or '--run'. Expected exactly 1 argument (file path)";
                            yall_exception.error(message, YALLErrorTypes::ValueError);
                        }
                        pos += 1;
                        yall.run(args[0]);
                        exit(0);
                    },
                    { "-r main.yall", "--run main.yall" }
                },
                {
                    { "-v", "--version" },
                    "Show program version information",
                    [this](const vector<string>& args, int* pos){
                        cout << yall_rainbow.apply({ "bold" }, NAME) << " " << yall_rainbow.apply({ "yellow" }, VERSION) << endl;
                        exit(0);
                    },
                    { "-v", "--version" }
                } 
            }
        };
    }

    void handle(int argc, char* argv[]) {
        for (int pos = 1; pos < argc; pos++) {
            const string input_option = string(argv[pos]);

            for (int index = 0; index < option_list.size(); index++) {
                const YALLOption option_data = option_list[index];
                const vector<string> args(argv, argv + argc);

                for (const string& option : option_data.options) {
                    if (option == input_option) option_data.callback(yall_modder.slice_vector(args, index + 1, args.size()), &pos);
                }
            }
        }
        this->show_help();
        exit(0);
    }
};

YALLCLI yall_cli;