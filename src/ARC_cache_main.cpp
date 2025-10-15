#include <iostream>
#include <vector>
#include <optional>
#include "ARC_cache.h"

int main () {

    std::ios_base::sync_with_stdio(false); // for cin speed boost
    std::cin.tie(nullptr); // detach cin from cout
    size_t capacity, n;
    std::cin >> capacity >> n;

    std::vector<int> requests(n);
    for (auto& key : requests) {
        std::cin >> key;
    }
    
    ARC_cache<int, int> cache(capacity);
    int cache_hits = 0;
    for (auto& key : requests) {
   
        std::optional<std::reference_wrapper<int>> res = cache.get(key);
        if (!res) 
            cache.put(key, key);

        else
            cache_hits++;
    }

    std::cout << cache_hits;

    return 0; 
}
