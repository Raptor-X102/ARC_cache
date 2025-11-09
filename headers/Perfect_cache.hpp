#pragma once

#include <map>
#include <unordered_map>
#include <optional>
#include <queue>
#include <iostream>
#include <vector>
#include "Timer.hpp"

template <typename T>
class Perfect_cache {
private:
    size_t capacity_;
    std::vector<std::pair<int, T>> requests_;
    std::unordered_map<int, std::queue<size_t>> key_entries_;
    std::map<int, T> cache_;
    int cache_hits_ = 0;

public:
    Perfect_cache(size_t capacity, std::vector<std::pair<int, T>>& requests)
        : capacity_(capacity), requests_(requests) {
        build_key_entries_map_();
    }
    
    void dump_cache() {

        std::cout << "Cache: ";
        for (const auto& [k, v] : cache_)
            std::cout << "(" << k << ", " << v << ") ";
        
        std::cout << std::endl;
    }

    int run() {
        
        #ifdef TIME_MEASURE
        Timer timer;
        #endif

        if (capacity_ == 0) return 0;

        for (size_t i = 0; i < requests_.size(); i++) {

            process_request_(i);
            // dump_cache();
        }
        
        return cache_hits_;
    }

private:

    void process_request_(size_t request_index) {

        const auto& [key, value] = requests_[request_index];
        update_key_entries_(key);

        if (handle_cache_hit_(key, value))
            return;

        if (!key_entries_.at(key).empty())
            handle_cache_miss_(key, value, request_index);
    }

    bool handle_cache_hit_(int key, const T& value) {

        auto cache_it = cache_.find(key);
        if (cache_it == cache_.end())
            return false;
        
        if (cache_it->second == value)
            cache_hits_++;

        else
            cache_it->second = value;
        
        return true;
    }

    void handle_cache_miss_(int key, const T& value, size_t current_index) {

        if (cache_.size() < capacity_) {

            cache_.emplace(key, value);
            return;
        }

        auto victim = find_victim_(key, current_index);
        if (victim.has_value()) {

            cache_.erase(victim.value());
            cache_.emplace(key, value);
        }
    }

    std::optional<int> find_victim_(int new_key, size_t current_index) {
        auto farthest_entry = find_farthest_used_key_();

        if (farthest_entry.unused_key.has_value())
            return farthest_entry.unused_key;

        return select_best_victim_(new_key, farthest_entry);
    }

    struct Farthest_entry_result {

        std::optional<int> unused_key;
        size_t farthest_distance = 0;
        int farthest_key = -1;
    };

    Farthest_entry_result find_farthest_used_key_() const {

        Farthest_entry_result result;
        for (const auto& [key, _] : cache_) {

            if (key_entries_.at(key).empty()) {

                result.unused_key = key;
                return result; 
            }

            size_t next_use = key_entries_.at(key).front();
            if (next_use > result.farthest_distance) {

                result.farthest_distance = next_use;
                result.farthest_key = key;
            }
        }

        return result;
    }

    std::optional<int> select_best_victim_(int new_key, const Farthest_entry_result& farthest) {

        if (farthest.farthest_key == -1)
            return std::nullopt;

        size_t new_key_next_use = key_entries_.at(new_key).front();

        if (farthest.farthest_distance > new_key_next_use)
            return farthest.farthest_key;
        
        return std::nullopt;
    }

    void build_key_entries_map_() {

        for (size_t i = 0; i < requests_.size(); i++)
            key_entries_[requests_[i].first].push(i);
    }

    void update_key_entries_(int key) {

        if (!key_entries_[key].empty())
            key_entries_[key].pop();
    }
};
