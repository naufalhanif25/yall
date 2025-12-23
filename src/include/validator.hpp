#pragma once

#include "header.hpp"

class YALLValidator {
public:
    inline bool is_number(const string& str) noexcept {
        const size_t num = str.size();
        if (num == 0) return false;
        bool has_dot = false;
        const char* p = str.data();

        for (size_t index = 0; index < num; ++index) {
            const char ch = p[index];
            if (ch >= '0' && ch <= '9') continue;
            if (ch == '.' && !has_dot && index != 0 && index + 1 < num) {
                has_dot = true;
                continue;
            }
            return false;
        }
        return true;
    }

    bool is_string(const string& str) {
        if (str.size() < 2) return false;
        char q = str.front();
        return (q == '"' || q == '\'') && str.back() == q;
    }

    bool is_bool(const string& str) { return str == "true" || str == "false"; }
    bool is_null(const string& str) { return str == "null"; }
    bool is_label(const string& str) { return !str.empty() && str.back() == ':'; }

    inline bool is_name_valid(const string& str) noexcept {
        if (str.empty()) return false;
        unsigned char ch = str[0];
        if (!((ch | 32) >= 'a' && (ch | 32) <= 'z')) return false;

        for (size_t index = 1; index < str.size(); ++index) {
            ch = str[index];
            if (((ch | 32) >= 'a' && (ch | 32) <= 'z') || (ch >= '0' && ch <= '9') || ch == '_') continue;
            return false;
        }
        return true;
    }

    bool is_instruction(const string& str) {
        const char* p = str.data();
        const char* end = p + str.size();
        if (p == end) return false;
        bool expect_name = true;

        while (p < end) {
            char ch = *p;
            
            if (ch == '(') return p != str.data() && end[-1] == ')';
            if (ch == '.') {
                if (expect_name) return false;
                expect_name = true;
                ++p;
                continue;
            }
            if (expect_name) {
                if (!isalpha(ch)) return false;
                expect_name = false;
            } 
            else if (!(isalpha(ch) || isdigit(ch) || ch == '_')) return false;
            ++p;
        }
        return false;
    }

    bool is_whitespace(const string& str) { return str.size() == 1 && isspace(static_cast<unsigned char>(str[0])); }

    bool is_comment(const string& str) {
        for (char ch : str) if (!isspace(static_cast<unsigned char>(ch))) return ch == '#';
        return false;
    }

    bool is_goto(const string& str) {
        const string prefix = "goto<>";
        if (str.size() <= prefix.size()) return false;
        if (str.compare(0, 5, "goto<") != 0) return false;
        if (str.back() != '>') return false;
        return str[5] != '>';
    }

    bool is_child_placeholder(const string& str) {
        const string prefix = "child<>";
        if (str.size() <= prefix.size()) return false;
        if (str.compare(0, 6, "child<") != 0) return false;
        if (str.back() != '>') return false;
        for (size_t index = 6; index + 1 < str.size(); ++index) if (!isdigit(str[index])) return false;
        return true;
    }

    bool is_set(const string& str) {
        size_t length = str.size();
        if (length < 2) return false;
        if (str.front() != '<' || str.back() != '>') return false;
        bool in_single_quote = false, in_double_quote = false;
        int depth = 0;

        for (size_t index = 1; index + 1 < length; ++index) {
            char ch = str[index];

            if (ch == '\'' && !in_double_quote) in_single_quote = !in_single_quote;
            else if (ch == '"' && !in_single_quote) in_double_quote = !in_double_quote;
            else if (!in_single_quote && !in_double_quote) {
                if (ch == '<') depth++;
                else if (ch == '>') if (depth-- == 0) return false;
            }
        }
        return !in_single_quote && !in_double_quote && depth == 0;
    }
};

extern YALLValidator yall_validator;
