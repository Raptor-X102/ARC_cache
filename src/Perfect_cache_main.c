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
    
    int cache_hits = 0;
    std::vector<std::pair<int, int>> pair_requests(n);
    for (size_t i = 0; i < requests.size(); ++i) 
        pair_requests[i] = {requests[i], requests[i]};
     
    cache_hits = Perfect_cache(pair_requests, capacity);
    std::cout << cache_hits;
    
    return 0;
}
