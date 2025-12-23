#pragma once

#include "header.hpp"
#include "validator.hpp"

class YALLModder {
public:
    string quote_string(const string& str) {
        if (str.empty() || yall_validator.is_string(str)) return str;
        return ("\"" + str + "\"");
    }

    string trim_quotes(const string& str) {
        if (str.size() < 2) return str;
        char first = str.front();
        char last  = str.back();
        if ((first == '\'' && last == '\'') || (first == '"'  && last == '"')) return str.substr(1, str.size() - 2);
        return str;
    }

    string trim_whitespace(const string& str) {
        size_t start = 0, end = str.size();
        while (start < end && isspace(static_cast<unsigned char>(str[start]))) start++;
        while (end > start && isspace(static_cast<unsigned char>(str[end - 1]))) end--;
        return str.substr(start, end - start);
    }

    string_view trim_whitespace_view(const string& str) {
        size_t start = 0, end = str.size();
        while (start < end && isspace(static_cast<unsigned char>(str[start]))) ++start;
        while (end > start && isspace(static_cast<unsigned char>(str[end - 1]))) --end;
        return string_view(str.data() + start, end - start);
    }

    string trim_goto(const string& str) {
        size_t start = str.find('<');
        size_t end = str.find('>');
        if (start == string::npos || end == string::npos || end <= start + 1) return "";
        return str.substr(start + 1, end - start - 1);
    }

    string trim_label(const string& str) {
        if (!str.empty() && str.back() == ':') return str.substr(0, str.size() - 1);
        return str;
    }

    inline string eval_unescape(const string& str) {
        const char* src = str.data();
        const char* end = src + str.size();
        string result;
        result.reserve(str.size());

        while (src < end) {
            char ch = *src++;
            if (ch == '\\' && src < end) {
                switch (*src++) {
                    case 'n':  result.push_back('\n'); break;
                    case 't':  result.push_back('\t'); break;
                    case 'r':  result.push_back('\r'); break;
                    case '\\': result.push_back('\\'); break;
                    case '\'': result.push_back('\''); break;
                    case '"':  result.push_back('"');  break;
                    case '0':  result.push_back('\0'); break;
                    default:
                        result.push_back('\\');
                        result.push_back(src[-1]);
                        break;
                }
            } 
            else result.push_back(ch);
        }
        return result;
    }

    string join_vector(const vector<string>& vec, const string& delimiter = ".") {
        string result;
        result.reserve(64);
        bool first = true;

        for (const auto& vec : vec) {
            string temp = this->trim_quotes(vec);
            if (yall_validator.is_string(temp)) temp = this->trim_quotes(temp);
            if (temp.empty()) continue;
            if (!first) result.append(delimiter);
            result.append(temp);
            first = false;
        }
        return result;
    }

    template <typename T> vector<T> slice_vector(const vector<T>& vec, size_t start, size_t end) {
        if (start >= vec.size() || start > end) return {};
        end = min(end, vec.size() - 1);
        return vector<T>(vec.begin() + start, vec.begin() + end + 1);
    }

    string to_goto(const string& label) { return "goto<" + label + ">"; }

    string get_goto_label(const string& placeholder) {
        const string prefix = "goto<";
        const char suffix = '>';

        if (placeholder.size() <= prefix.size() + 1) return "";
        if (placeholder.compare(0, prefix.size(), prefix) != 0) return "";
        if (placeholder.back() != suffix) return "";
        return placeholder.substr(prefix.size(), placeholder.size() - prefix.size() - 1);
    }

    string to_child(const int& index) { return "child<" + to_string(index) + ">"; }

    string get_child_index(const string& placeholder) {
        const string prefix = "child<";
        const char suffix = '>';
        
        if (placeholder.size() <= prefix.size() + 1) return "";
        if (placeholder.compare(0, prefix.size(), prefix) != 0) return "";
        if (placeholder.back() != suffix) return "";
        if (!yall_validator.is_child_placeholder(placeholder)) return "";
        return placeholder.substr(prefix.size(), placeholder.size() - prefix.size() - 1);
    }

    vector<string> parse_set(const string& input) {
        vector<string> result;

        if (input.size() < 2 || input.front() != '<' || input.back() != '>') return result;
        result.reserve(8);
        size_t start = 1, depth = 0;
        bool in_single = false, in_double = false;

        for (size_t index = 1; index < input.size() - 1; ++index) {
            char ch = input[index];

            if (ch == '\'' && !in_double) in_single = !in_single;
            else if (ch == '"' && !in_single) in_double = !in_double;
            else if (!in_single && !in_double) {
                if (ch == '(') depth++;
                else if (ch == ')') depth--;
                else if (ch == ',' && depth == 0) {
                    result.emplace_back(this->trim_whitespace(input.substr(start, index - start)));
                    start = index + 1;
                }
            }
        }
        if (start < input.size() - 1) result.emplace_back(this->trim_whitespace(input.substr(start, input.size() - 1 - start)));
        return result;
    }

    string build_set(const vector<string>& args) {
        if (args.empty()) return "<>";
        size_t total_len = 2;
        for (const string& arg : args) total_len += arg.size();
        total_len += args.size() - 1;
        string result;

        result.reserve(total_len);
        result.push_back('<');

        for (size_t index = 0; index < args.size(); ++index) {
            result += args[index];
            if (index + 1 < args.size()) result.push_back(',');
        }
        result.push_back('>');
        return result;
    }

    inline int stoi(const string& str) noexcept {
        int value = 0, sign = 1;
        size_t index = 0, len = str.size();

        if (len == 0) return 0;
        if (str[0] == '-') {
            sign = -1;
            index = 1;
        }
        for (; index < len; ++index) value = value * 10 + (str[index] - '0');
        return value * sign;
    }

    inline long long stoll(const string& str) noexcept {
        long long value = 0;
        size_t index = 0;
        bool negative = false;

        if (str.empty()) return 0;
        if (str[index] == '-') {
            negative = true;
            ++index;
        } 
        else if (str[index] == '+') ++index;
        if (index >= str.size() || str[index] < '0' || str[index] > '9') return 0;

        while (index < str.size()) {
            char ch = str[index];
            if (ch < '0' || ch > '9') break;
            value = value * 10 + (ch - '0');
            ++index;
        }
        return negative ? -value : value;
    }

    inline double stod(const string& str) noexcept {
        const char* cstr = str.c_str();
        size_t index = 0, len = str.size();
        if (len == 0) return 0.0;

        bool negative = false;
        if (cstr[index] == '-') {
            negative = true;
            ++index;
        } 
        else if (cstr[index] == '+') ++index;

        double value = 0.0;
        while (index < len) {
            char ch = cstr[index];
            if (ch < '0' || ch > '9') break;
            value = value * 10.0 + (ch - '0');
            ++index;
        }
        if (index < len && cstr[index] == '.') {
            ++index;
            double factor = 0.1;
            while (index < len) {
                char ch = cstr[index];
                if (ch < '0' || ch > '9') break;
                value += (ch - '0') * factor;
                factor *= 0.1;
                ++index;
            }
        }
        return negative ? -value : value;
    }

    inline string trim_trailing_zeros(string str) {
        auto pos = str.find('.');
        if (pos == string::npos) return str;

        size_t end = str.size();
        while (end > pos + 1 && str[end - 1] == '0') --end;
        if (end == pos + 1) --end;
        
        str.resize(end);
        return str;
    }
};

extern YALLModder yall_modder;