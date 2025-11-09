#ifndef ARC_CACHE
#define ARC_CACHE

#include <list>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <functional>    
#include "Debug_printf.h"
#include <cmath>

template <typename T, typename KeyT = int>
class ARC_cache {
    
    class Adaptive_parameter {
    private:
        int p_ = 0;

    public:
        Adaptive_parameter() = default;
        explicit Adaptive_parameter(int p) : p_(p) {}

        void increase(ARC_cache* owner) {

            if (owner->B1_.empty()) return;

            int adjustment = std::max(1, static_cast<int>(std::round(
                static_cast<double>(owner->B2_.size()) / owner->B1_.size()
            )));
            p_ = std::min(
                 p_ + adjustment,
                 static_cast<int>(owner->capacity_)
            );
        }

        void decrease(ARC_cache* owner) {
            if (owner->B2_.empty()) return;

            int adjustment = std::max(1, static_cast<int>(std::round(
                static_cast<double>(owner->B1_.size()) / owner->B2_.size()
            )));
            p_ = std::max(p_ - adjustment, 0);
        }

        int get() const { 
        
            return p_; 
        }
    };

    size_t capacity_;

    std::list<std::pair<KeyT, T>> T1_; // Recently used
    std::list<std::pair<KeyT, T>> T2_; // Frequently used
    std::list<std::pair<KeyT, T>> B1_; // evicted from T1_  
    std::list<std::pair<KeyT, T>> B2_; // evicted from T2_

    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator> T1_map_;
    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator> T2_map_;
    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator> B1_map_;
    std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator> B2_map_;

    Adaptive_parameter adapt_par_; 

public:

    ARC_cache() : capacity_(10), adapt_par_(0) {}
    explicit ARC_cache(size_t capacity) : capacity_(capacity), adapt_par_(0) {}

    ARC_cache(const ARC_cache&) = delete;
    ARC_cache& operator=(const ARC_cache&) = delete;
    ARC_cache(ARC_cache&&) = delete;
    ARC_cache& operator=(ARC_cache&&) = delete;
    ~ARC_cache() = default;

    void dump() {

        std::cout << "capacity: " << capacity_ << std::endl;
        std::cout << "adapt_par: " << adapt_par_.get() << std::endl;
        std::cout << "T1: "; dump_list_(T1_);
        std::cout << "T2: "; dump_list_(T2_);
        std::cout << "B1: "; dump_list_(B1_);
        std::cout << "B2: "; dump_list_(B2_);
        std::cout << std::endl;
    }

    std::optional<std::reference_wrapper<T>> get(KeyT key) {

        if (auto result = find_and_promote_in_cache_(key)) {
            return result;
        }

        return std::nullopt;
    }

    void put(KeyT key, T value) {

        if (find_and_promote_in_cache_(key)) {
            return;
        }

        if (handle_ghost_hit_(key)) {
            return;
        }

        insert_new_element_(key, std::move(value));
    }

private:
    
    void rebuild_maps_from_(const ARC_cache& rhs) {

        T1_map_.clear();
        for (auto it = T1_.begin(); it != T1_.end(); ++it) 
            T1_map_[it->first] = it;

        T2_map_.clear();
        for (auto it = T2_.begin(); it != T2_.end(); ++it)
            T2_map_[it->first] = it;

        B1_map_.clear();
        for (auto it = B1_.begin(); it != B1_.end(); ++it)
            B1_map_[it->first] = it;
        

        B2_map_.clear();
        for (auto it = B2_.begin(); it != B2_.end(); ++it)
            B2_map_[it->first] = it;
    }

    std::optional<std::reference_wrapper<T>> find_and_promote_in_cache_(KeyT key) {

        if (auto it = T1_map_.find(key); it != T1_map_.end()) {

            //DEBUG_PRINTF("FOUND IN T1\n");
            promote_from_T1_to_T2_(it->second);
            return std::ref(T2_.front().second);
        }

        if (auto it = T2_map_.find(key); it != T2_map_.end()) {

            //DEBUG_PRINTF("FOUND IN T2\n");
            promote_within_T2_(it->second);
            return std::ref(T2_.front().second);
        }

        return std::nullopt;
    }

    bool handle_ghost_hit_(KeyT key) {

        if (auto it = B1_map_.find(key); it != B1_map_.end()) {

            //DEBUG_PRINTF("FOUND IN B1\n");
            handle_B1_hit_(it->second);
            return true;
        }

        if (auto it = B2_map_.find(key); it != B2_map_.end()) {

            //DEBUG_PRINTF("FOUND IN B2\n");
            handle_B2_hit_(it->second);
            return true;
        }

        return false;
    }

    void insert_new_element_(KeyT key, T&& value) {

        if (T1_.size() + T2_.size() >= capacity_)
            evict_();
        
        T1_.push_front({key, std::move(value)});
        T1_map_[key] = T1_.begin();
    }

    void promote_from_T1_to_T2_(typename std::list<std::pair<KeyT, T>>::iterator it) {

        auto& [k, v] = *it;
        T2_.splice(T2_.begin(), T1_, it);
        T1_map_.erase(k);
        T2_map_[k] = T2_.begin();
    }

    void promote_within_T2_(typename std::list<std::pair<KeyT, T>>::iterator it) {

        auto& [k, v] = *it;
        T2_.splice(T2_.begin(), T2_, it);
        T2_map_[k] = T2_.begin();
    }

    void handle_B1_hit_(typename std::list<std::pair<KeyT, T>>::iterator it) {

        auto& [k, v] = *it;
        adapt_par_.increase(this);
        B1_map_.erase(k);
        T2_.splice(T2_.begin(), B1_, it);
        T2_map_[k] = T2_.begin();
        maintain_cache_size_();
    }

    void handle_B2_hit_(typename std::list<std::pair<KeyT, T>>::iterator it) {

        auto& [k, v] = *it;
        adapt_par_.decrease(this);
        B2_map_.erase(k);
        T2_.splice(T2_.begin(), B2_, it);
        T2_map_[k] = T2_.begin();
        maintain_cache_size_();
    }

    void maintain_cache_size_() {

        while (T1_.size() + T2_.size() > capacity_) 
            evict_();
    }

    
    void evict_() {

        if (should_evict_from_T1_())
            evict_from_T1_();
        
        else
            evict_from_T2_();
    }

    bool should_evict_from_T1_() const {

        return T1_.size() > std::max(1, adapt_par_.get());
    }

    void evict_from_T1_() {

        evict_from_list_(T1_, T1_map_, B1_, B1_map_);
    }

    void evict_from_T2_() {

        if (T2_.empty()) return;
        evict_from_list_(T2_, T2_map_, B2_, B2_map_);
    }

    void evict_from_list_(std::list<std::pair<KeyT, T>>& from_list,
                        std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator>& from_map,
                        std::list<std::pair<KeyT, T>>& to_list,
                        std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator>& to_map) {

        auto it = std::prev(from_list.end());
        auto& [k, v] = *it;
        
        to_list.splice(to_list.begin(), from_list, it);
        from_map.erase(k);
        to_map[k] = to_list.begin();

        trim_ghost_list_(to_list, to_map);
    }

    void trim_ghost_list_(std::list<std::pair<KeyT, T>>& list,
                        std::unordered_map<KeyT, typename std::list<std::pair<KeyT, T>>::iterator>& map) {

        if (list.size() > capacity_) {

            auto last = std::prev(list.end());
            map.erase(last->first);
            list.erase(last);
        }
    }

    void dump_list_(const std::list<std::pair<KeyT, T>>& list) {

        for (const auto& [k, v] : list)
            std::cout << "(" << k << ", " << v << ") ";
        
        std::cout << std::endl;
    }
};

#endif
