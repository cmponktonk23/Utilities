#include "cache.h"

namespace utilities {

template <typename KeyType, typename ValueType>
auto CacheSystem<KeyType, ValueType>::Put(const KeyType &key, const ValueType &value) -> bool {
  std::lock_guard<std::mutex> lock(cache_mutex_);

  auto it = kv_map_.find(key);
  if (it == kv_map_.end()) {
    if (kv_map_.size() >= capacity_) {
      auto victim_key = replacement_policy_->Evict();
      // evict failed
      if (!victim_key.has_value()) {
        // return false;
        throw CacheException("Evict failed");
      }
      kv_map_.erase(victim_key.value());
    }
    // add failed
    if (!replacement_policy_->Add(key)) {
      // return false;
      throw CacheException("Add key failed");
    }
    kv_map_[key] = value;
  } else {
    // update failed
    if (!replacement_policy_->Access(key)) {
      // return false;
      throw CacheException("Update key failed");
    }
    it->second = value;
  }
  return true;
}

template <typename KeyType, typename ValueType>
auto CacheSystem<KeyType, ValueType>::Get(const KeyType &key) -> std::optional<ValueType> {
  std::lock_guard<std::mutex> lock(cache_mutex_);

  auto it = kv_map_.find(key);
  // not found
  if (it == kv_map_.end()) {
    // return std::nullopt;
    throw CacheException("Key not found");
  }

  // update failed
  if (!replacement_policy_->Access(key)) {
    // return std::nullopt;
    throw CacheException("Update key failed");
  }

  return it->second;
}

template <typename KeyType, typename ValueType>
auto CacheSystem<KeyType, ValueType>::Remove(const KeyType &key) -> bool {
  std::lock_guard<std::mutex> lock(cache_mutex_);

  auto it = kv_map_.find(key);
  // not found
  if (it == kv_map_.end()) {
    // return false;
    throw CacheException("Key not found");
  }

  // remove failed
  if (!replacement_policy_->Remove(key)) {
    // return false;
    throw CacheException("Remove key failed");
  }

  kv_map_.erase(it);
  return true;
}

template class CacheSystem<int, int>;

}  // namespace utilities