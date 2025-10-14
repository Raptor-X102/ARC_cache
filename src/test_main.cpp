#include <iostream>
#include <vector>
#include <optional>
#include "ARC_cache.h"
#include "Perfect_cache.h"

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
    cache.dump();
    std::cout << std::endl;
    for (auto& key : requests) {
   
        std::cout << "get & put: " << key << std::endl;
        std::optional<std::reference_wrapper<int>> res = cache.get(key);

        if (!res) {

            std::cout << "Miss!" << std::endl;
            cache.put(key, key);
        } 

        else {

            std::cout << "Hit!" << std::endl;
            cache_hits++;
        }

        //cache.dump();
        std::cout << std::endl;
    }

    std::cout << cache_hits << std::endl;

    std::vector<std::pair<int, int>> pair_requests(n);
    for (size_t i = 0; i < requests.size(); ++i) 
        pair_requests[i] = {requests[i], requests[i]};
     
    cache_hits = Perfect_cache(pair_requests, capacity);
    std::cout << cache_hits << std::endl;
}
