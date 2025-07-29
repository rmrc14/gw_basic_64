#include <gtest/gtest.h>
#include <string>
#include <sstream>

#include "ErrorHandler.h"

// --- Mock SystemInterface for capturing output ---
namespace {
    std::ostringstream testOutput;

    // Override SystemInterface for testing
    namespace SystemInterface {
        void printString(const char* str) {
            testOutput << str;
        }
    }
}

// --- Actual Tests ---
TEST(ErrorHandlerTest, RuntimeErrorDirectMode) {
    testOutput.str(""); // Clear buffer
    ErrorHandler err;
    err.runtimeError(-1, "Division by zero");
    EXPECT_EQ(testOutput.str(), "[Runtime Error] (directMode): Division by zero\n");
}

TEST(ErrorHandlerTest, RuntimeErrorLineMode) {
    testOutput.str(""); // Clear buffer
    ErrorHandler err;
    err.runtimeError(20, "Invalid array index");
    EXPECT_EQ(testOutput.str(), "[Runtime Error] line 20 : Invalid array index\n");
}

TEST(ErrorHandlerTest, SyntaxErrorLineMode) {
    testOutput.str("");
    ErrorHandler err;
    err.syntaxError(40, "Missing THEN");
    EXPECT_EQ(testOutput.str(), "[Syntax Error] line 40 : Missing THEN\n");
}

TEST(ErrorHandlerTest, SyntaxErrorDirectMode) {
    testOutput.str("");
    ErrorHandler err;
    err.syntaxError(-1, "Unknown command");
    EXPECT_EQ(testOutput.str(), "[Syntax Error] (directMode): Unknown command\n");
}

TEST(ErrorHandlerTest, TypeError) {
    testOutput.str("");
    ErrorHandler err;
    err.typeError(60, "Mismatched types");
    EXPECT_EQ(testOutput.str(), "[Type Error] line 60 : Mismatched types\n");
}

TEST(ErrorHandlerTest, SystemError) {
    testOutput.str("");
    ErrorHandler err;
    err.systemError("File not found");
    EXPECT_EQ(testOutput.str(), "[System Error] : File not found\n");
}
