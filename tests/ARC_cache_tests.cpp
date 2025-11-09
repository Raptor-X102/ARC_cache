#include "ARC_cache.hpp"
#include <iostream>
#include <vector>

struct TestConfig {

    std::vector<int> keys;
    size_t capacity;
    int expected_hits;
    std::string description;
};

int total_tests = 0;
int failed_tests = 0;

#define RUN_TEST(config) do { \
    ++total_tests; \
    std::cout << "[" << total_tests << "] "; \
    run_test(config); \
} while(0)

void assert_equal(int expected, int actual, const std::string& test_name) {

    if (expected == actual) {
        std::cout << "PASS: " << test_name << "\n";
    } else {
        std::cout << "FAIL: " << test_name << " (expected: " << expected << ", got: " << actual << ")\n";
        ++failed_tests;
    }
}

void run_test(const TestConfig& config) {

    std::cout << "Test: " << config.description << "\n";
    std::cout << "Input: capacity=" << config.capacity << ", keys=[";
    for (size_t i = 0; i < config.keys.size(); ++i) {
        std::cout << config.keys[i];
        if (i < config.keys.size() - 1) std::cout << ",";
    }
    std::cout << "]\n";
    std::cout << "Expected hits: " << config.expected_hits << "\n";

    ARC_cache<int> cache(config.capacity);
    int hits = 0;
    for (int key : config.keys) {
        if (cache.get(key).has_value()) {
            hits++;
        } else {
            cache.put(key, key * 10);
        }
    }

    std::cout << "Actual hits: " << hits << "\n";
    assert_equal(config.expected_hits, hits, config.description);
    std::cout << "---\n";
}

int main() {

    std::vector<TestConfig> tests = {
        {{1, 2, 3, 1, 2, 3, 1, 2, 3, 4, 5, 6, 4, 5, 6}, 3, 8, "ARC Repeated Pattern"},
        {{1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8}, 4, 2, "ARC Large Cycle"},
        {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1, 9, "ARC Single Key Repeated"},
        {{1, 2, 3, 4, 5, 1, 2, 3, 4, 5}, 3, 2, "ARC Repeated After Capacity"},
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2}, 5, 0, "ARC Large Sequence"},
        {{1, 2, 1, 3, 1, 4, 1, 5, 1, 6}, 2, 1, "ARC Alternating with 1"},
        {{1, 2, 3, 2, 3, 1, 2, 3, 4, 5, 4, 5}, 3, 7, "ARC Mixed Pattern"},
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10, 0, "ARC All Unique"},
        {{1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5}, 5, 10, "ARC Full Capacity Reuse"},
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 1, 0, "ARC Capacity 1 Long Sequence"}
    };

    for (const auto& test : tests) {
        RUN_TEST(test);
    }

    std::cout << "\nTotal tests: " << total_tests << ", Failed: " << failed_tests << "\n";
    return failed_tests;
}
