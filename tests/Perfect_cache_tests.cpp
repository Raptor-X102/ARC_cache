#include <iostream>
#include <vector>
#include <cassert>
#include "Perfect_cache.hpp"

struct IdealTest {

    const char* name;
    size_t capacity;
    std::vector<int> keys;
    int expected_hits;
};

void run_ideal_tests() {

    std::vector<IdealTest> tests = {
        {"Ideal Repeated Pattern", 3, {1,2,3,1,2,3,1,2,3,4,5,6,4,5,6}, 9},
        {"Ideal Large Cycle", 4, {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8}, 4},
        {"Ideal Single Key Repeated", 1, {1,1,1,1,1,1,1,1,1,1}, 9},
        {"Ideal Repeated After Capacity", 3, {1,2,3,4,5,1,2,3,4,5}, 3},
        {"Ideal Large Sequence", 5, {1,2,3,4,5,6,7,8,9,10,1,2}, 2},
        {"Ideal Alternating with 1", 2, {1,2,1,3,1,4,1,5,1,6}, 4},
        {"Ideal Mixed Pattern", 3, {1,2,3,2,3,1,2,3,4,5,4,5}, 7},
        {"Ideal All Unique", 10, {1,2,3,4,5,6,7,8,9,10}, 0},
        {"Ideal Full Capacity Reuse", 5, {1,2,3,4,5,1,2,3,4,5,1,2,3,4,5}, 10},
        {"Ideal Capacity 1 Long Sequence", 1, {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, 0},
        {"Ideal Simple Optimal", 2, {1,2,3,2,1}, 2},
        {"Ibel Future Knowledge", 3, {1,2,3,4,1,2,5,1,2,3,4}, 5}
    };

    int total_tests = tests.size();
    int failed_tests = 0;

    for (int i = 0; i < total_tests; ++i) {
        const auto& test = tests[i];
        
        std::vector<std::pair<int, int>> requests;
        for (auto key : test.keys) {
            requests.push_back({key, key});
        }
        
        int hits = Perfect_cache(requests, test.capacity);

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

    run_ideal_tests();
    return 0;
}
