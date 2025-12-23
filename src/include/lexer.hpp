#pragma once

#include "global.hpp"

class YALLLexer : protected YALLGlobal {
private:
    unordered_map<string, string> line_cache;

protected:
    struct YALLTokenData {
        int line;
        string token;
    };

    vector<string> split_lines(const string& data, char delimiter) {
        vector<string> result;
        result.reserve(16);
        size_t start = 0, len = data.size();

        for (size_t index = 0; index < len; ++index) {
            if (data[index] == delimiter) {
                string_view view(data.data() + start, index - start);
                string trimmed = yall_modder.trim_whitespace(string(view));
                if (!trimmed.empty()) result.push_back(trimmed);
                start = index + 1;
            }
        }
        if (start <= len) {
            string_view view(data.data() + start, len - start);
            string trimmed = yall_modder.trim_whitespace(string(view));
            if (!trimmed.empty()) result.push_back(trimmed);
        }
        return result;
    }

    vector<YALLTokenData> tokenize(const vector<string>& lines) {
        vector<YALLTokenData> tokens;
        tokens.reserve(lines.size() * 2);

        for (size_t index = 0; index < lines.size(); index++) {
            string line = lines[index]; 
            int current_line = index + 1;
            auto it = line_cache.find(line);
            if (it != line_cache.end()) tokens.push_back({ current_line, it->second });
            else {
                size_t start = 0;
                bool comment = false;
            
                for (size_t index = 0; index < line.size(); ++index) {
                    if (yall_validator.is_comment(string(1, line[index]))) {
                        string_view view(line.data() + start, index - start);
                        string trimmed = yall_modder.trim_whitespace(string(view));
                        
                        if (!trimmed.empty()) {
                            line_cache.insert({ line, trimmed });
                            tokens.push_back({ current_line, trimmed });
                        }
                        comment = true;
                        break;
                    }
                }
                if (!comment) {
                    string trimmed = yall_modder.trim_whitespace(line);
                    
                    if (!trimmed.empty()) {
                        line_cache.insert({ line, trimmed });
                        tokens.push_back({ current_line, trimmed });
                    }
                }
            }
        }
        return tokens;
    }

public:
    YALLLexer() {
        this->line_cache = {};
    }

    vector<YALLTokenData> lex(const string& lines) { 
        vector<YALLTokenData> tokens = this->tokenize(this->split_lines(lines, '\n'));
        line_cache.clear();
        return tokens; 
    }
};