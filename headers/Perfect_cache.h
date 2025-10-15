#pragma once

#include <set>
#include <unordered_map>
#include <queue>

template <typename T>
void dump_cache(std::set<std::pair<int, T>> & cache) {

    std::cout << "Cache: ";
    for (const auto& [k, v] : cache) {

        std::cout << "(" << k << ", " << v << ") ";
    }
    std::cout << std::endl;
}

template <typename T>
int Perfect_cache(std::vector<std::pair<int, T>> & requests, size_t capacity) {

    std::unordered_map<int, std::queue<size_t>> key_entries;
    for (size_t i = 0; i < requests.size(); i++) 
        key_entries[requests[i].first].push(i);

    std::set<std::pair<int, T>> cache;

    int cache_hits = 0;
    //dump_cache(cache);
    for (size_t i = 0; i < requests.size(); i++) {

        auto [key, val] = requests[i];
        key_entries[key].pop();

        if (cache.count(requests[i])) {

            cache_hits++; 
        }

        else {

            if (cache.size() >= capacity) {

                size_t farthest_entry = 0;
                auto farthest_it = cache.begin();
                for (auto it = cache.begin(); it != cache.end(); it++) {

                    auto& [k, v] = *it;
                    if (key_entries[k].empty()) {
                        farthest_it = it;
                        break;
                    }

                    size_t curr_farthest_entry = key_entries[k].front();
                    if (curr_farthest_entry > farthest_entry) {
                        farthest_entry = curr_farthest_entry;
                        farthest_it = it;
                    }
                }

                if (farthest_it != cache.end()) 
                    cache.erase(farthest_it);
            }

            cache.insert({key, val});
        }

        //dump_cache(cache);
    }
    
    return cache_hits;
}
