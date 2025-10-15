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
        ARC_cache* owner_;  

    public:
        explicit Adaptive_parameter(ARC_cache* owner) : owner_(owner) {}

        void increase() {

            if (owner_->B1_.empty()) return;

            int adjustment = std::max(1, static_cast<int>(std::round(
                static_cast<double>(owner_->B2_.size()) / owner_->B1_.size()
            )));
            owner_->p_ = std::min(
                owner_->p_ + adjustment,
                static_cast<int>(owner_->capacity_)
            );
        }

        void decrease() {
            if (owner_->B2_.empty()) return;

            int adjustment = std::max(1, static_cast<int>(std::round(
                static_cast<double>(owner_->B1_.size()) / owner_->B2_.size()
            )));
            owner_->p_ = std::max(owner_->p_ - adjustment, 0);
        }

        int get() const { 
        
            return owner_->p_; 
        }
    };

    size_t capacity_;
    int p_ = 0;

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

    ARC_cache() : ARC_cache(10) {};

    explicit ARC_cache(size_t capacity) : capacity_(capacity), adapt_par_(this) {}

    ARC_cache(const ARC_cache& rhs) : 

        capacity_(rhs.capacity_), adapt_par_(this),
        T1_(rhs.T1_), T2_(rhs.T2_), B1_(rhs.B1_), B2_(rhs.B2_) {
        rebuild_maps_from(rhs);
    }
    
    ARC_cache& operator=(const ARC_cache& rhs) {

        if (this != &rhs) {

            capacity_ = rhs.capacity_;
            T1_ = rhs.T1_;
            T2_ = rhs.T2_;
            B1_ = rhs.B1_;
            B2_ = rhs.B2_;
            rebuild_maps_from(rhs);
        }
        
        return *this;
    }

    ARC_cache(ARC_cache&& rhs) noexcept : // no need to renew iterators as they are valid after move
        capacity_(rhs.capacity_), adapt_par_(this),
        T1_(std::move(rhs.T1_)), T2_(std::move(rhs.T2_)),
        B1_(std::move(rhs.B1_)), B2_(std::move(rhs.B2_)),
        T1_map_(std::move(rhs.T1_map_)), T2_map_(std::move(rhs.T2_map_)),
        B1_map_(std::move(rhs.B1_map_)), B2_map_(std::move(rhs.B2_map_)) {}


    ARC_cache& operator=(ARC_cache&& rhs) noexcept {  
        
        if (this != &rhs) {

            capacity_ = rhs.capacity_;
            T1_ = std::move(rhs.T1_);
            T2_ = std::move(rhs.T2_);
            B1_ = std::move(rhs.B1_);
            B2_ = std::move(rhs.B2_);
            T1_map_ = std::move(rhs.T1_map_);
            T2_map_ = std::move(rhs.T2_map_);
            B1_map_ = std::move(rhs.B1_map_);
            B2_map_ = std::move(rhs.B2_map_);
        }
        return *this;
    }

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

    void move_to_T2(KeyT key, T&& val) {

        T2_.push_front({key, std::move(val)});
        T2_map_[key] = T2_.begin();
    }

    void evict() {

        if (T1_.size() > std::max(1, adapt_par_.get())) {

            auto it = std::prev(T1_.end());
            auto& [k, v] = *it;
            B1_.splice(B1_.begin(), T1_, it);
            T1_map_.erase(k);
            B1_map_[k] = B1_.begin();

            if (B1_.size() > capacity_) {

                auto last = std::prev(B1_.end());
                B1_map_.erase(last->first);
                B1_.erase(last);
            }
        } 

        else {

            if (T2_.empty()) 
                return;

            auto it = std::prev(T2_.end());
            auto& [k, v] = *it;
            B2_.splice(B2_.begin(), T2_, it);
            T2_map_.erase(k);
            B2_map_[k] = B2_.begin();

            if (B2_.size() > capacity_) {

                auto last = std::prev(B2_.end());
                B2_map_.erase(last->first);
                B2_.erase(last);
            }
        }
    }

    std::optional<std::reference_wrapper<T>> get(KeyT key) {

        //DEBUG_PRINTF("GET\n");
        auto it1 = T1_map_.find(key);
        if (it1 != T1_map_.end()) {

            //DEBUG_PRINTF("FOUND IN T1\n");
            auto list_it = it1->second;
            auto& [k, v] = *list_it;
            T2_.splice(T2_.begin(), T1_, list_it);
            T1_map_.erase(k);
            T2_map_[k] = T2_.begin();
            return std::ref(T2_.begin()->second);
        }

        auto it2 = T2_map_.find(key);
        if (it2 != T2_map_.end()) {

            //DEBUG_PRINTF("FOUND IN T2\n");
            auto list_it = it2->second;
            auto& [k, v] = *list_it;
            T2_.splice(T2_.begin(), T2_, list_it);
            T2_map_[k] = T2_.begin();
            return std::ref(T2_.begin()->second);
        }

        //DEBUG_PRINTF("NOT FOUND IN T1 OR T2\n");
        return std::nullopt;
    }

    void put(KeyT key, T value) {

        //DEBUG_PRINTF("PUT\n");
        auto it1 = T1_map_.find(key);
        if (it1 != T1_map_.end()) {

            //DEBUG_PRINTF("FOUND IN T1\n");
            auto list_it = it1->second;
            auto& [k, v] = *list_it;
            T2_.splice(T2_.begin(), T1_, list_it);
            T1_map_.erase(k);
            T2_map_[k] = T2_.begin();
            return;
        }

        auto it2 = T2_map_.find(key);
        if (it2 != T2_map_.end()) {

            //DEBUG_PRINTF("FOUND IN T2\n");
            auto list_it = it2->second;
            auto& [k, v] = *list_it;
            T2_.splice(T2_.begin(), T2_, list_it);
            T2_map_[k] = T2_.begin();
            return;
        }

        auto itB1 = B1_map_.find(key);
        if (itB1 != B1_map_.end()) {

            //DEBUG_PRINTF("FOUND IN B1\n");
            auto list_it = itB1->second;
            auto& [k, v] = *list_it;
            adapt_par_.increase();
            B1_map_.erase(k);
            T2_.splice(T2_.begin(), B1_, list_it);
            T2_map_[k] = T2_.begin();
            while (T1_.size() + T2_.size() > capacity_) evict();
            return;
        }

        auto itB2 = B2_map_.find(key);
        if (itB2 != B2_map_.end()) {

            //DEBUG_PRINTF("FOUND IN B2\n");
            auto list_it = itB2->second;
            auto& [k, v] = *list_it;
            adapt_par_.decrease();
            B2_map_.erase(k);
            T2_.splice(T2_.begin(), B2_, list_it);
            T2_map_[k] = T2_.begin();
            while (T1_.size() + T2_.size() > capacity_) evict();
            return;
        }
        
        if (T1_.size() + T2_.size() >= capacity_) 
            evict();
        //DEBUG_PRINTF("NOT FOUND\n");
        T1_.push_front({key, std::move(value)});
        T1_map_[key] = T1_.begin();
    }

private:
    
    void rebuild_maps(const ARC_cache& rhs) {

        T1_map_.clear();
        for (auto& [key, it] : rhs.T1_map_) {

            auto new_it = std::next(T1_.begin(), std::distance(rhs.T1_.begin(), it));
            T1_map_[key] = new_it;
        }

        T2_map_.clear();
        for (auto& [key, it] : rhs.T2_map_) {

            auto new_it = std::next(T2_.begin(), std::distance(rhs.T2_.begin(), it));
            T2_map_[key] = new_it;
        }

        B1_map_.clear();
        for (auto& [key, it] : rhs.B1_map_) {

            auto new_it = std::next(B1_.begin(), std::distance(rhs.B1_.begin(), it));
            B1_map_[key] = new_it;
        }

        B2_map_.clear();
        for (auto& [key, it] : rhs.B2_map_) {

            auto new_it = std::next(B2_.begin(), std::distance(rhs.B2_.begin(), it));
            B2_map_[key] = new_it;
        }
    }
    void dump_list_(std::list<std::pair<KeyT, T>> & list) {

        for(auto [k, v] : list) 
            std::cout << "(" << k << ", " << v << ") ";

        std::cout << std::endl;
    }
};


#endif
