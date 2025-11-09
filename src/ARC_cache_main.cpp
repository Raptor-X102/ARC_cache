#include <iostream>
#include <vector>
#include <optional>
#include "ARC_cache.hpp"
#include "Timer.hpp"

int main () {

    std::ios_base::sync_with_stdio(false); // for cin speed boost
    std::cin.tie(nullptr); // detach cin from cout
    ssize_t capacity, n;
    std::cin >> capacity >> n;

    if (capacity <= 0) {

        DEBUG_PRINTF("ERROR: invalid cache size\n");
        return 1;
    }

    if (n <= 0) {

        DEBUG_PRINTF("ERROR: invalid input keys amount\n");
        return 1;
    }

    std::vector<int> requests(n);
    for (auto& key : requests) {
        std::cin >> key;
    }
    
    ARC_cache<int, int> cache(capacity);
    int cache_hits = 0;
    
    #ifdef TIME_MEASURE
    {
        Timer timer;
    #endif

        for (auto& key : requests) {
       
            std::optional<std::reference_wrapper<int>> res = cache.get(key);
            if (!res) 
                cache.put(key, key);

            else
                cache_hits++;
        }

    #ifdef TIME_MEASURE
    }
    #endif

    std::cout << cache_hits;

    return 0; 
}
