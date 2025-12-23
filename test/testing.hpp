#pragma once

#include "header.hpp"

class YALLTestReport {
private:
    pair<int, int> total;

public:
    YALLTestReport() { this->total = { 0, 0 }; } 
    
    void passed() { this->total.first++; }
    void error() { this->total.second++; }

    void sum() {
        cout << yall_rainbow.apply({ "cyan", "bold" }, "Summary: ") << endl;
        cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "green" }, "Passed: ") << this->total.first << endl;
        cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "red" }, "Failed: ") << this->total.second << endl;
    }
};

YALLTestReport report;

class YALLTestExpect {
public:
    YALLTestExpect(int value) : intValue(value), type(Type::INT) {}
    YALLTestExpect(const string& value) : strValue(value), type(Type::STRING) {}

    void tobe(int expected) {
        if (type != Type::INT) {
            report.error();
            cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "red" }, "[\u2717] ") << "Type mismatch: expected INT" << endl;
            return;
        }
        if (intValue == expected) {
            report.passed();
            cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "green" }, "[\u2713] ") << "Passed" << endl;
        }
        else {
            report.error();
            cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "red" }, "[\u2717] ") << ("Expected " + to_string(expected) + ", got " + to_string(intValue)) << endl;
        }
    }

    void tobe(const string& expected) {
        if (type != Type::STRING) {
            report.error();
            cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "red" }, "[\u2717] ") << "Type mismatch: expected STRING" << endl;
            return;
        }
        if (strValue == expected) {
            report.passed();
            cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "green" }, "[\u2713] ") << "Passed" << endl;
        }
        else {
            report.error();
            cout << yall_rainbow.apply({ "dim", "bold" }, "\u2192 ") << yall_rainbow.apply({ "red" }, "[\u2717] ") << ("Expected \"" + expected + "\", got \"" + strValue + "\"") << endl;
        }
    }

private:
    enum class Type { INT, STRING };
    Type type;
    int intValue{};
    string strValue;
};

YALLTestExpect expect(int value) { return YALLTestExpect(value); }
YALLTestExpect expect(const string& value) { return YALLTestExpect(value); }

class YALLTestRunner {
public:
    void add(const string& name, const function<void()>& func) { tests.push_back({ name, func }); }

    void run() {
        for (auto &test : tests) {
            cout << yall_rainbow.apply({ "cyan", "bold" }, "Test: ") << test.name << endl;
            test.func();
            cout << endl;
        }
        report.sum();
    }

private:
    struct YALLTestItem {
        string name;
        function<void()> func;
    };
    vector<YALLTestItem> tests;
};

class YALLTest {
public:
    YALLTestRunner runner;

    YALLTest() { this->runner; } 
};

YALLTest yall_test;

void test(const string& name, const function<void()>& callback) { yall_test.runner.add(name, callback); }