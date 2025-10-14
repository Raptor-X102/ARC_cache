#include <iostream>
#include <vector>
#include <cassert>
#include "ARC_cache.h"

struct ARCTest {
    const char* name;
    size_t capacity;
    std::vector<int> keys;
    int expected_hits;
};

void run_arc_tests() {
    std::vector<ARCTest> tests = {
        {"ARC Repeated Pattern", 3, {1,2,3,1,2,3,1,2,3,4,5,6,4,5,6}, 6},
        {"ARC Large Cycle", 4, {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8}, 2},
        {"ARC Single Key Repeated", 1, {1,1,1,1,1,1,1,1,1,1}, 9},
        {"ARC Repeated After Capacity", 3, {1,2,3,4,5,1,2,3,4,5}, 1},
        {"ARC Large Sequence", 5, {1,2,3,4,5,6,7,8,9,10,1,2}, 0},
        {"ARC Alternating with 1", 2, {1,2,1,3,1,4,1,5,1,6}, 4},
        {"ARC Mixed Pattern", 3, {1,2,3,2,3,1,2,3,4,5,4,5}, 5},
        {"ARC All Unique", 10, {1,2,3,4,5,6,7,8,9,10}, 0},
        {"ARC Full Capacity Reuse", 5, {1,2,3,4,5,1,2,3,4,5,1,2,3,4,5}, 10},
        {"ARC Capacity 1 Long Sequence", 1, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, 0},
        {"ARC Simple Hit", 2, {1,2,1}, 1},
        {"ARC Multiple Hits", 2, {1,2,1,2,1}, 3},
        {"ARC Sequential", 3, {1,2,3,4,1,2,3,4}, 0}
    };

    int total_tests = tests.size();
    int failed_tests = 0;

    for (int i = 0; i < total_tests; ++i) {
        const auto& test = tests[i];
        ARC_cache<int, int> cache(test.capacity);
        int hits = 0;

        for (auto key : test.keys) {
            if (cache.get(key).has_value()) {
                hits++;
            } else {
                cache.put(key, key);
            }
        }

        if (hits == test.expected_hits) {
            std::cout << "[PASS] Test " << i+1 << ": " << test.name << std::endl;
            std::cout << "  Input: capacity=" << test.capacity << ", keys=[";
            for (size_t j = 0; j < std::min(test.keys.size(), size_t(10)); ++j) {
                if (j > 0) std::cout << ",";
                std::cout << test.keys[j];
            }
            if (test.keys.size() > 10) std::cout << "...";
            std::cout << "]" << std::endl;
            std::cout << "  Expected hits: " << test.expected_hits << std::endl;
            std::cout << "  Actual hits: " << hits << std::endl;
        } else {
            std::cout << "[FAIL] Test " << i+1 << ": " << test.name << std::endl;
            std::cout << "  Input: capacity=" << test.capacity << ", keys=[";
            for (size_t j = 0; j < std::min(test.keys.size(), size_t(10)); ++j) {
                if (j > 0) std::cout << ",";
                std::cout << test.keys[j];
            }
            if (test.keys.size() > 10) std::cout << "...";
            std::cout << "]" << std::endl;
            std::cout << "  Expected hits: " << test.expected_hits << std::endl;
            std::cout << "  Actual hits: " << hits << std::endl;
            failed_tests++;
        }
        std::cout << "---" << std::endl;
    }

    std::cout << "Total tests: " << total_tests << ", Failed: " << failed_tests << std::endl;
}

int main() {
    run_arc_tests();
    return 0;
}
