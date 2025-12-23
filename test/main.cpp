#include "testing.hpp"
#include "../src/include/yall.hpp"

int main () {
    test("[Validator] String: Detect quoted strings correctly", [](){
        expect(yall_validator.is_string("\"Hello World\"")).tobe(true);
        expect(yall_validator.is_string("\'Hello World\'")).tobe(true);
        expect(yall_validator.is_string("Hello World")).tobe(false);
    });

    test("[Validator] Number: Validate integer and float formats", [](){
        expect(yall_validator.is_number("25")).tobe(true);
        expect(yall_validator.is_number("2.5")).tobe(true);
        expect(yall_validator.is_number(".5")).tobe(false);
        expect(yall_validator.is_number("Hello World")).tobe(false);
    });

    test("[Validator] Boolean: Accept lowercase true/false only", [](){
        expect(yall_validator.is_bool("true")).tobe(true);
        expect(yall_validator.is_bool("false")).tobe(true);
        expect(yall_validator.is_bool("True")).tobe(false);
        expect(yall_validator.is_bool("False")).tobe(false);
    });

    test("[Validator] Null: Recognize 'null' literal", [](){
        expect(yall_validator.is_null("null")).tobe(true);
        expect(yall_validator.is_null("Null")).tobe(false);
    });

    test("[Validator] Name: Validate naming conventions (lower, camel, pascal, snake, upper)", [](){
        expect(yall_validator.is_name_valid("programname")).tobe(true);
        expect(yall_validator.is_name_valid("programName")).tobe(true);
        expect(yall_validator.is_name_valid("ProgramName")).tobe(true);
        expect(yall_validator.is_name_valid("program_name")).tobe(true);
        expect(yall_validator.is_name_valid("PROGRAMNAME")).tobe(true);
        expect(yall_validator.is_name_valid("program name")).tobe(false);
    });

    test("[Validator] Label: Must end with ':'", [](){
        expect(yall_validator.is_label("main:")).tobe(true);
        expect(yall_validator.is_label("main")).tobe(false);
    });

    test("[Validator] Instruction: Validate format name(args...)", [](){
        expect(yall_validator.is_instruction("instructionname(any())")).tobe(true);
        expect(yall_validator.is_instruction("instructionName(any())")).tobe(true);
        expect(yall_validator.is_instruction("InstructionName(any())")).tobe(true);
        expect(yall_validator.is_instruction("instruction_name(any())")).tobe(true);
        expect(yall_validator.is_instruction("INSTRUCTIONNAME(any())")).tobe(true);
        expect(yall_validator.is_instruction("instruction.name(any())")).tobe(true);
        expect(yall_validator.is_instruction("instruction name(any())")).tobe(false);
    });

    test("[Validator] Whitespace: Detect space / tab / newline / carriage-return", [](){
        expect(yall_validator.is_whitespace(" ")).tobe(true);
        expect(yall_validator.is_whitespace("\t")).tobe(true);
        expect(yall_validator.is_whitespace("\n")).tobe(true);
        expect(yall_validator.is_whitespace("\r")).tobe(true);
        expect(yall_validator.is_whitespace("A")).tobe(false);
    });

    test("[Validator] Comment: Must start with '#'", [](){
        expect(yall_validator.is_comment("#")).tobe(true);
        expect(yall_validator.is_comment("# This is a comment")).tobe(true);
        expect(yall_validator.is_comment("### This is a comment")).tobe(true);
        expect(yall_validator.is_comment("This is not a comment #")).tobe(false);
        expect(yall_validator.is_comment("This is not a comment")).tobe(false);
    });

    test("[Validator] Goto: Validate 'goto<value>' format", [](){
        expect(yall_validator.is_goto("goto<hello>")).tobe(true);
        expect(yall_validator.is_goto("goto<>")).tobe(false);
    });

    test("[Validator] Child Placeholder: Validate 'child<n>' format", [](){
        expect(yall_validator.is_child_placeholder("child<1>")).tobe(true);
        expect(yall_validator.is_child_placeholder("child<100>")).tobe(true);
        expect(yall_validator.is_child_placeholder("child<2.5>")).tobe(false);
        expect(yall_validator.is_child_placeholder("child<abc>")).tobe(false);
        expect(yall_validator.is_child_placeholder("child<>")).tobe(false);
    });

    test("[Validator] is_set(): validate set format", [](){
        expect(yall_validator.is_set("<>")).tobe(true);
        expect(yall_validator.is_set("<1,2,3>")).tobe(true);
        expect(yall_validator.is_set("<\"a\",\"b\",\"c\">")).tobe(true);
        expect(yall_validator.is_set("<'a','b','c'>")).tobe(true);
        expect(yall_validator.is_set("<<'a','b'>,<'c','d'>>")).tobe(true);
        expect(yall_validator.is_set("<\"<1,2>\", \"<3,4>\">")).tobe(true);
        expect(yall_validator.is_set("")).tobe(false);
        expect(yall_validator.is_set("<")).tobe(false);
        expect(yall_validator.is_set("<1,2,3")).tobe(false);
        expect(yall_validator.is_set("[1,2,3]")).tobe(false);
        expect(yall_validator.is_set("{1,2,3}")).tobe(false);
    });

    test("[Modder] quote_string(): basic cases", [](){
        expect(yall_modder.quote_string("hello")).tobe("\"hello\"");
        expect(yall_modder.quote_string("")).tobe("");
        expect(yall_modder.quote_string("\"hello\"")).tobe("\"hello\"");
        expect(yall_modder.quote_string("'hello'")).tobe("'hello'");
    });

    test("[Modder] trim_quotes(): remove surrounding quotes", [](){
        expect(yall_modder.trim_quotes("\"hello\"")).tobe("hello");
        expect(yall_modder.trim_quotes("'world'")).tobe("world");
        expect(yall_modder.trim_quotes("\"hello")).tobe("\"hello");
        expect(yall_modder.trim_quotes("hello\"")).tobe("hello\"");
        expect(yall_modder.trim_quotes("h")).tobe("h");
    });

    test("[Modder] trim_whitespace(): remove leading and trailing spaces", [](){
        expect(yall_modder.trim_whitespace("  hello  ")).tobe("hello");
        expect(yall_modder.trim_whitespace("\t hello\n")).tobe("hello");
        expect(yall_modder.trim_whitespace("hello")).tobe("hello");
        expect(yall_modder.trim_whitespace("   ")).tobe("");
    });

    test("[Modder] trim_whitespace_view(): view trimming correctness", [](){
        string input_1 = "  hello world  ";
        string_view sv = yall_modder.trim_whitespace_view(input_1);
        expect(string(sv)).tobe("hello world");
        string input_2 = "test";
        expect(string(yall_modder.trim_whitespace_view(input_2))).tobe("test");
        string input_3 = "   ";
        expect(string(yall_modder.trim_whitespace_view(input_3))).tobe("");
    });

    test("[Modder] trim_goto(): extract label from goto format", [](){
        expect(yall_modder.trim_goto("goto<main>")).tobe("main");
        expect(yall_modder.trim_goto("<loop>")).tobe("loop");
        expect(yall_modder.trim_goto("goto<>")).tobe("");
        expect(yall_modder.trim_goto("goto<")).tobe("");
        expect(yall_modder.trim_goto("goto")).tobe("");
    });

    test("[Modder] trim_label(): remove colon suffix", [](){
        expect(yall_modder.trim_label("main:")).tobe("main");
        expect(yall_modder.trim_label("loop:")).tobe("loop");
        expect(yall_modder.trim_label("main")).tobe("main");
        expect(yall_modder.trim_label("")).tobe("");
    });

    test("[Modder] eval_unescape(): unescape sequences", [](){
        expect(yall_modder.eval_unescape("\\n")).tobe("\n");
        expect(yall_modder.eval_unescape("\\t")).tobe("\t");
        expect(yall_modder.eval_unescape("\\r")).tobe("\r");
        expect(yall_modder.eval_unescape("\\\\")).tobe("\\");
        expect(yall_modder.eval_unescape("\\\"")).tobe("\"");
        expect(yall_modder.eval_unescape("\\0")).tobe(string(1, '\0'));
        expect(yall_modder.eval_unescape("abc")).tobe("abc");
    });

    test("[Modder] join_vector(): join and trim quoted strings", [](){
        expect(yall_modder.join_vector({"\"a\"", "\"b\""})).tobe("a.b");
        expect(yall_modder.join_vector({"'a'", "'b'"}, "-")).tobe("a-b");
        expect(yall_modder.join_vector({"", "\"x\""})).tobe("x");
        expect(yall_modder.join_vector({})).tobe("");
    });

    test("[Modder] to_goto(): generate goto placeholder", [](){
        expect(yall_modder.to_goto("main")).tobe("goto<main>");
    });
    
    test("[Modder] get_goto_label(): extract label from goto placeholder", [](){
        expect(yall_modder.get_goto_label("goto<main>")).tobe("main");
    });

    test("[Modder] to_child(): generate child placeholder", [](){
        expect(yall_modder.to_child(0)).tobe("child<0>");
        expect(yall_modder.to_child(5)).tobe("child<5>");
        expect(yall_modder.to_child(123)).tobe("child<123>");
    });
    
    test("[Modder] get_child_index(): extract index from child placeholder", [](){
        expect(yall_modder.get_child_index("child<0>")).tobe("0");
        expect(yall_modder.get_child_index("child<123>")).tobe("123");
        expect(yall_modder.get_child_index("child<>")).tobe("");
        expect(yall_modder.get_child_index("child<abc>")).tobe("");
    });

    test("[Modder] parse_set() & build_set(): valid and invalid set parsing", []() {
        {
            auto result = yall_modder.parse_set("<1,2,3>");
            expect(result.size()).tobe(3);
            expect(result[0]).tobe("1");
            expect(result[1]).tobe("2");
            expect(result[2]).tobe("3");
        }
        {
            auto result = yall_modder.parse_set("< 1 ,  2 , 3 >");
            expect(result.size()).tobe(3);
            expect(result[0]).tobe("1");
            expect(result[1]).tobe("2");
            expect(result[2]).tobe("3");
        }
        {
            auto result = yall_modder.parse_set("<\"a\",\"b\",\"c\">");
            expect(result.size()).tobe(3);
            expect(result[0]).tobe("\"a\"");
            expect(result[1]).tobe("\"b\"");
            expect(result[2]).tobe("\"c\"");
        }
        {
            auto result = yall_modder.parse_set("<func(1,2), func(3,4)>");
            expect(result.size()).tobe(2);
            expect(result[0]).tobe("func(1,2)");
            expect(result[1]).tobe("func(3,4)");
        }

        expect(yall_modder.parse_set("<>").size()).tobe(0);
        expect(yall_modder.parse_set("").size()).tobe(0);
        expect(yall_modder.parse_set("<").size()).tobe(0);
        expect(yall_modder.parse_set(">").size()).tobe(0);
        expect(yall_modder.parse_set("[1,2,3]").size()).tobe(0);
        expect(yall_modder.parse_set("<1,2,3").size()).tobe(0);
        expect(yall_modder.parse_set("1,2,3>").size()).tobe(0);
        expect(yall_modder.build_set({ "1", "2", "3" })).tobe("<1,2,3>");
        expect(yall_modder.build_set({})).tobe("<>");
        expect(yall_modder.build_set({ "\"a\"", "\"b\"" })).tobe("<\"a\",\"b\">");
        
        {
            string input = "<1, 2, func(3,4)>";
            auto parsed = yall_modder.parse_set(input);
            string rebuilt = yall_modder.build_set(parsed);
            expect(rebuilt).tobe("<1,2,func(3,4)>");
        }
    });

    test("[Modder] stoi(): fast integer parsing", []() {
        expect(yall_modder.stoi("0")).tobe(0);
        expect(yall_modder.stoi("1")).tobe(1);
        expect(yall_modder.stoi("123")).tobe(123);
        expect(yall_modder.stoi("99999")).tobe(99999);
        expect(yall_modder.stoi("-1")).tobe(-1);
        expect(yall_modder.stoi("-42")).tobe(-42);
        expect(yall_modder.stoi("-98765")).tobe(-98765);
        expect(yall_modder.stoi("000123")).tobe(123);
        expect(yall_modder.stoi("-0007")).tobe(-7);
        expect(yall_modder.stoi("12a")).tobe(12 * 10 + ('a' - '0'));
        expect(yall_modder.stoi("a12")).tobe(('a' - '0') * 100 + 12);
        expect(yall_modder.stoi("")).tobe(0);
    });

    test("[Modder] stoll(): fast long long parsing", []() {
        expect(yall_modder.stoll("")).tobe(0);
        expect(yall_modder.stoll("+")).tobe(0);
        expect(yall_modder.stoll("-")).tobe(0);
        expect(yall_modder.stoll("abc")).tobe(0);
        expect(yall_modder.stoll("12abc")).tobe(12);
        expect(yall_modder.stoll("0")).tobe(0);
        expect(yall_modder.stoll("+0")).tobe(0);
        expect(yall_modder.stoll("-0")).tobe(0);
        expect(yall_modder.stoll("1")).tobe(1);
        expect(yall_modder.stoll("42")).tobe(42);
        expect(yall_modder.stoll("123456")).tobe(123456);
        expect(yall_modder.stoll("-1")).tobe(-1);
        expect(yall_modder.stoll("-42")).tobe(-42);
        expect(yall_modder.stoll("-987654")).tobe(-987654);
        expect(yall_modder.stoll("+123")).tobe(123);
        expect(yall_modder.stoll("-123abc")).tobe(-123);
        expect(yall_modder.stoll("999xyz")).tobe(999);
    });

    test("[Modder] stod(): fast double parsing", []() {
        expect(yall_modder.stod("")).tobe(0.0);
        expect(yall_modder.stod("+")).tobe(0.0);
        expect(yall_modder.stod("-")).tobe(0.0);
        expect(yall_modder.stod("abc")).tobe(0.0);
        expect(yall_modder.stod("0")).tobe(0.0);
        expect(yall_modder.stod("+0")).tobe(0.0);
        expect(yall_modder.stod("-0")).tobe(0.0);
        expect(yall_modder.stod("1")).tobe(1.0);
        expect(yall_modder.stod("42")).tobe(42.0);
        expect(yall_modder.stod("-1")).tobe(-1.0);
        expect(yall_modder.stod("+123")).tobe(123.0);
        expect(yall_modder.stod("1.0")).tobe(1.0);
        expect(yall_modder.stod("3.14")).tobe(3.14);
        expect(yall_modder.stod("0.5")).tobe(0.5);
        expect(yall_modder.stod(".5")).tobe(0.0);
        expect(yall_modder.stod("5.")).tobe(5.0);
        expect(yall_modder.stod("12.34abc")).tobe(12.34);
        expect(yall_modder.stod("-12.34xyz")).tobe(-12.34);
        expect(yall_modder.stod("999.999zzz")).tobe(999.999);
        expect(yall_modder.stod("00012.3400")).tobe(12.34);
    });

    test("[Modder] trim_trailing_zeros(): remove trailing decimal zeros", []() {
        expect(yall_modder.trim_trailing_zeros("1.2")).tobe("1.2");
        expect(yall_modder.trim_trailing_zeros("-3.14")).tobe("-3.14");
        expect(yall_modder.trim_trailing_zeros("1.0")).tobe("1");
        expect(yall_modder.trim_trailing_zeros("-5.0000")).tobe("-5");
        expect(yall_modder.trim_trailing_zeros("12.340")).tobe("12.34");
        expect(yall_modder.trim_trailing_zeros("-99.9900")).tobe("-99.99");
        expect(yall_modder.trim_trailing_zeros("0.100")).tobe("0.1");
        expect(yall_modder.trim_trailing_zeros("0.000")).tobe("0");
    });

    yall_test.runner.run();
    exit(0);
}