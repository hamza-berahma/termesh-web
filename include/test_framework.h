/**
 * @file test_framework.h
 * @brief Simple test framework for unit testing.
 */

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

/**
 * @brief Simple test framework class for organizing unit tests.
 */
class TestFramework {
public:
    struct TestResult {
        std::string name;
        bool passed;
        std::string message;
    };

    static TestFramework& instance() {
        static TestFramework instance;
        return instance;
    }

    void addTest(const std::string& name, bool passed, const std::string& message = "") {
        results.push_back({name, passed, message});
        if (passed) {
            std::cout << "✓ " << name << std::endl;
        } else {
            std::cout << "✗ " << name;
            if (!message.empty()) {
                std::cout << " - " << message;
            }
            std::cout << std::endl;
        }
    }

    void printSummary() {
        int passed = 0;
        int failed = 0;
        for (const auto& result : results) {
            if (result.passed) {
                passed++;
            } else {
                failed++;
            }
        }

        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Passed: " << passed << std::endl;
        std::cout << "Failed: " << failed << std::endl;
        std::cout << "Total: " << results.size() << std::endl;

        if (failed > 0) {
            std::cout << "\nFailed tests:" << std::endl;
            for (const auto& result : results) {
                if (!result.passed) {
                    std::cout << "  - " << result.name;
                    if (!result.message.empty()) {
                        std::cout << ": " << result.message;
                    }
                    std::cout << std::endl;
                }
            }
        }
    }

    int getExitCode() const {
        for (const auto& result : results) {
            if (!result.passed) {
                return 1;
            }
        }
        return 0;
    }

    bool wasTestAdded(const std::string& name) const {
        for (const auto& result : results) {
            if (result.name == name) {
                return true;
            }
        }
        return false;
    }

    void clearResults() {
        results.clear();
    }

private:
    std::vector<TestResult> results;
};

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            TestFramework::instance().addTest(__FUNCTION__, false, #condition " is false"); \
            return; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            TestFramework::instance().addTest(__FUNCTION__, false, #condition " is true"); \
            return; \
        } \
    } while(0)

#define ASSERT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            TestFramework::instance().addTest(__FUNCTION__, false, \
                std::string(#a " != " #b " (got ") + std::to_string(a) + " vs " + std::to_string(b) + ")"); \
            return; \
        } \
    } while(0)

#define ASSERT_FLOAT_EQ(a, b, epsilon) \
    do { \
        if (std::abs((a) - (b)) >= (epsilon)) { \
            TestFramework::instance().addTest(__FUNCTION__, false, \
                std::string(#a " != " #b " (got ") + std::to_string(a) + " vs " + std::to_string(b) + ")"); \
            return; \
        } \
    } while(0)

#define ASSERT_VEC3_EQ(a, b, epsilon) \
    do { \
        if (!(a).isApproxEqual(b, epsilon)) { \
            TestFramework::instance().addTest(__FUNCTION__, false, \
                "Vec3 " #a " != " #b); \
            return; \
        } \
    } while(0)

#define RUN_TEST(testFunc) \
    do { \
        bool testPassed = true; \
        std::string testName = #testFunc; \
        try { \
            testFunc(); \
        } catch (...) { \
            testPassed = false; \
            if (!TestFramework::instance().wasTestAdded(testName)) { \
                TestFramework::instance().addTest(testName, false, "Exception thrown"); \
            } \
        } \
        if (!TestFramework::instance().wasTestAdded(testName)) { \
            TestFramework::instance().addTest(testName, testPassed); \
        } \
    } while(0)

