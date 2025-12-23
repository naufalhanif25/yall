#pragma once

#include "executor.hpp"

class YALLParser : protected YALLExecutor {
private:
    struct YALLArgsKey {
        string args;

        bool operator==(const YALLArgsKey& other) const { 
            return args == other.args; 
        }
    };

    struct YALLArgsHash {
        size_t operator()(const YALLArgsKey& key) const {
            size_t hashed = hash<string>{}(key.args);
            return hashed;
        }
    };

    unordered_map<YALLArgsKey, vector<string>, YALLArgsHash> args_cache;
    unordered_map<string, YALLParsedInstruction> instruction_cahce;

    inline pair<string_view, string_view> split_module_instruction(string_view str) noexcept {
        size_t pos = str.find('.');
        if (pos == string_view::npos) return { str, {} };
        return { str.substr(0, pos), str.substr(pos + 1) };
    }

protected:
    vector<string> split_args(const string& args) {
        vector<string> result;
        YALLArgsKey key = { args };
        auto it = args_cache.find(key);

        if (it != args_cache.end()) result = it->second;
        else {
            result.reserve(8);
            bool in_single_quote = false, in_double_quote = false;
            size_t depth = 0, start = 0, length = args.size();
    
            for (size_t index = 0; index < length; ++index) {
                char ch = args[index];
    
                if (ch == '\'' && !in_double_quote) in_single_quote = !in_single_quote;
                else if (ch == '"' && !in_single_quote) in_double_quote = !in_double_quote;
                else if (!in_single_quote && !in_double_quote) {
                    if (ch == '(') depth++;
                    else if (ch == ')') depth--;
                    else if (ch == ',' && depth == 0) {
                        string_view view(args.data() + start, index - start);
                        result.push_back(yall_modder.trim_whitespace(string(view)));
                        start = index + 1;
                    }
                }
            }
            if (start < length) {
                string_view view(args.data() + start, length - start);
                result.push_back(yall_modder.trim_whitespace(string(view)));
            }
            args_cache.emplace(key, result);
        }
        return result;
    }

    YALLParsedInstruction parse_instruction(const string& input, const int& line) {
        YALLParsedInstruction result;
        auto it = instruction_cahce.find(input);

        if (it != instruction_cahce.end()) result = it->second;
        else {
            string_view str = yall_modder.trim_whitespace_view(input);
            size_t pos = str.find('('), end = str.rfind(')');
            if (pos == string::npos || end == string::npos || end < pos) return result;
            
            result.set_name(yall_modder.trim_whitespace(string(str.substr(0, pos))));
            result.set_line(line);
            pair<string_view, string_view> instruction_parts = this->split_module_instruction(string_view(result.name));
            result.set_parts(string(instruction_parts.first), string(instruction_parts.second));

            string_view inside = str.substr(pos + 1, end - pos - 1);
            vector<string> rawArgs = split_args(string(inside));
    
            result.args.reserve(rawArgs.size());
            result.children.reserve(rawArgs.size());
    
            for (auto& arg : rawArgs) {
                if (yall_validator.is_instruction(arg)) {
                    size_t index = result.children.size();
                    result.push_child(parse_instruction(arg, line));
                    result.push_arg(yall_modder.to_child(index));
                }
                else if (!arg.empty()) result.args.push_back(arg);
            }
            instruction_cahce.insert({ input, result });
        }
        return result;
    }

    string eval_instruction(const YALLParsedInstruction& instruction, const string& label, int depth = 0) {
        vector<string> evaluatedArgs;
        evaluatedArgs.reserve(instruction.args.size());
        
        for (const auto& arg : instruction.args) {
            if (yall_validator.is_child_placeholder(arg)) {
                size_t index = stoul(yall_modder.get_child_index(arg));
                evaluatedArgs.push_back(eval_instruction(instruction.children[index], label, depth + 1));
            }
            else evaluatedArgs.push_back(arg);
        }
        return this->execute(instruction.name, instruction.parts, evaluatedArgs, label);
    }
    
public:
    YALLParser() {
        this->args_cache = {};
    }

    vector<YALLTokens> parse(const string& data) {
        vector<YALLTokenData> tokens = this->lex(data);
        vector<YALLTokens> result;
        size_t tokens_size = tokens.size();
        result.reserve(tokens_size);
        YALLTokens current_tokens;

        for (int index = 0; index < tokens.size(); index++) {
            YALLTokenData current_token = tokens[index];
            const string& token = current_token.token;
            current_tokens.set_line(current_token.line);

            if (yall_validator.is_label(token)) {
                if (!current_tokens.empty()) result.push_back(move(current_tokens));
                current_tokens = YALLTokens{};
                current_tokens.set_label(yall_modder.trim_label(token));
            }
            else if (yall_validator.is_instruction(token)) current_tokens.push_instruction(this->parse_instruction(token, current_tokens.line));
            else {
                string message = "Unexpected instruction '" + token + "': not a valid label or instruction";
                yall_exception.error(message, YALLErrorTypes::TokenError, current_tokens.line);
            }
        }
        if (!current_tokens.empty()) result.push_back(move(current_tokens));

        instruction_cahce.clear();
        args_cache.clear();

        return result;
    }

};