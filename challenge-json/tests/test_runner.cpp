// Simple C++ test runner for the JSON parser (no external dependencies).
// Builds a lightweight assertion framework and verifies parser behavior
// against provided fixtures in tests/step1..step4.
//
// How to build and run (after Makefile update in a later step):
//   make jp_tests
//   ./jp_tests
//
// Returns 0 on success (all tests pass), non-zero otherwise.

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "file_utils.h"
#include "lexer.h"
#include "parser.h"

struct TestCase {
    std::string name;
    std::string path;      // if empty, use inline JSON string
    std::string inlineJson;
    bool expect;
};

static bool runParse(const std::string& name, const std::string& json, bool expectValid) {
    try {
        Lexer lexer(json);
        Parser parser(std::move(lexer));
        bool ok = parser.parse();
        bool pass = (ok == expectValid);
        std::cout << (pass ? "[PASS] " : "[FAIL] ") << name
                  << " (expected " << (expectValid ? "valid" : "invalid")
                  << ", got " << (ok ? "valid" : "invalid") << ")\n";
        return pass;
    } catch (const std::exception& e) {
        std::cout << "[ERROR] " << name << " threw exception: " << e.what() << "\n";
        return false;
    } catch (...) {
        std::cout << "[ERROR] " << name << " threw unknown exception\n";
        return false;
    }
}

static bool runFile(const std::string& name, const std::string& path, bool expectValid) {
    try {
        std::string content = FileUtils::readFileForParsing(path);
        return runParse(name + " (" + path + ")", content, expectValid);
    } catch (const std::exception& e) {
        std::cout << "[ERROR] " << name << " failed to read '" << path << "': " << e.what() << "\n";
        return false;
    }
}

int main() {
    std::vector<TestCase> cases = {
        // Fixture-based tests from provided steps
        {"Step1 valid",  "tests/step1/valid.json",  "", true},
        {"Step1 invalid","tests/step1/invalid.json","", false},

        {"Step2 valid",   "tests/step2/valid.json",   "", true},
        {"Step2 valid2",  "tests/step2/valid2.json",  "", true},
        {"Step2 invalid", "tests/step2/invalid.json", "", false},
        {"Step2 invalid2","tests/step2/invalid2.json","", false},

        {"Step3 valid",   "tests/step3/valid.json",   "", true},
        {"Step3 invalid", "tests/step3/invalid.json", "", false},

        {"Step4 valid",   "tests/step4/valid.json",   "", true},
        {"Step4 valid2",  "tests/step4/valid2.json",  "", true},
        {"Step4 invalid", "tests/step4/invalid.json", "", false},

        // Additional inline unit tests for coverage
        {"Inline: empty input", "", "", false},
        {"Inline: whitespace only", "", "   \n\t", false},
        {"Inline: empty object {}", "", "{}", true},
        {"Inline: simple key:string", "", "{\"a\":\"b\"}", true},
        {"Inline: simple key:number", "", "{\"n\":123}", true},
        {"Inline: nested object", "", "{\"o\":{\"k\":false}}", true},
        {"Inline: array of numbers", "", "{\"arr\":[1,2,3]}", true},
        {"Inline: array of mixed types", "", "{\"arr\":[null,true,42,\"x\"]}", true},
        {"Inline: trailing comma object", "", "{\"a\":1,}", false},
        {"Inline: trailing comma array", "", "{\"a\":[1,2,]}", false},
        {"Inline: missing colon", "", "{\"a\" 1}", false},
        {"Inline: missing closing brace", "", "{\"a\":1", false},
        {"Inline: invalid top-level (array)", "", "[1,2,3]", false}, // top-level must be object per parser.parse
        {"Inline: invalid token", "", "{\"a\":@}", false}
    };

    int passed = 0;
    int failed = 0;

    std::cout << "Running C++ unit tests for JSON parser...\n";

    for (const auto& tc : cases) {
        bool ok = false;
        if (!tc.path.empty()) {
            ok = runFile(tc.name, tc.path, tc.expect);
        } else {
            ok = runParse(tc.name, tc.inlineJson, tc.expect);
        }
        if (ok) ++passed; else ++failed;
    }

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
