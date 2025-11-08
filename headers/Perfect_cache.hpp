#pragma once

#include <map>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <vector>
#include "Timer.hpp"

template <typename T>
void dump_cache(std::map<int, T>& cache) {
    std::cout << "Cache: ";
    for (const auto& [k, v] : cache)
        std::cout << "(" << k << ", " << v << ") ";
    
    std::cout << std::endl;
}

template <typename T>
int Perfect_cache(std::vector<std::pair<int, T>>& requests, size_t capacity) {
    
    //Timer timer;

    if (capacity == 0) 
        return 0;

    std::unordered_map<int, std::queue<size_t>> key_entries;
    for (size_t i = 0; i < requests.size(); i++)
        key_entries[requests[i].first].push(i);

    std::map<int, T> cache;

    int cache_hits = 0;
    for (size_t i = 0; i < requests.size(); i++) {

        auto [key, val] = requests[i];
        key_entries[key].pop();
        auto it = cache.find(key);
        if (it != cache.end()) {

            if (it->second == val)
                cache_hits++;

            else
                it->second = val;
        }

        else if (!key_entries[key].empty()) {

            if (cache.size() < capacity) {

                cache[key] = val;
            }

            else {

                auto farthest_it = cache.end();
                size_t farthest_entry = 0;
                bool found_unused = false;
                
                for (auto it = cache.begin(); it != cache.end(); it++) {
                    
                    auto& [k, v] = *it;
                    if (key_entries[k].empty()) {

                        farthest_it = it;
                        found_unused = true;
                        break;
                    }
                    
                    size_t curr_farthest_entry = key_entries[k].front();
                    if (curr_farthest_entry > farthest_entry) {

                        farthest_entry = curr_farthest_entry;
                        farthest_it = it;
                    }
                }
                
                size_t current_key_next_use = key_entries[key].front();
                
                if (found_unused) {

                    cache.erase(farthest_it);
                    cache[key] = val;
                } 

                else if (farthest_it != cache.end() && farthest_entry > current_key_next_use) {

                    cache.erase(farthest_it);
                    cache[key] = val;
                }
            }
        }

        //dump_cache(cache);
    }
    
    return cache_hits;
}
