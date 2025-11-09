#include <iostream>
#include <vector>
#include <optional>
#include "ARC_cache.hpp"
#include "Perfect_cache.hpp"

int main () {

    std::ios_base::sync_with_stdio(false); // for cin speed boost
    std::cin.tie(nullptr); // detach cin from cout
    size_t capacity, n;
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
    
    ARC_cache<int, int> arc_cache(capacity);
    int arc_cache_hits = 0;
    arc_cache.dump();
    std::cout << std::endl;
    
    #ifdef TIME_MEASURE
    {
        Timer timer;
    #endif

        for (auto& key : requests) {
       
            std::cout << "get & put: " << key << std::endl;
            std::optional<std::reference_wrapper<int>> res = arc_cache.get(key);

            if (!res) {

                std::cout << "Miss!" << std::endl;
                arc_cache.put(key, key);
            } 

            else {

                std::cout << "Hit!" << std::endl;
                arc_cache_hits++;
            }

            //arc_cache.dump();
            std::cout << std::endl;
        }
    
    #ifdef TIME_MEASURE
    }
    #endif

    std::cout << arc_cache_hits << std::endl;

    std::vector<std::pair<int, int>> pair_requests(n);
    for (size_t i = 0; i < requests.size(); ++i) 
        pair_requests[i] = {requests[i], requests[i]};
     
    Perfect_cache<int> perfect_cache(capacity, pair_requests); 
    int perfect_cache_hits = perfect_cache.run();
    std::cout << perfect_cache_hits;
}
