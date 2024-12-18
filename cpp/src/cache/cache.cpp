#include "cache.h"

namespace utilities {

template <typename KeyType, typename ValueType>
auto CacheSystem<KeyType, ValueType>::Put(const KeyType &key, const ValueType &value) -> bool {
  auto it = kv_map_.find(key);
  if (it == kv_map_.end()) {
    if (kv_map_.size() >= capacity_) {
      auto victim_key = replacement_policy_->Evict();
      // evict failed
      if (!victim_key.has_value()) {
        return false;
      }
      kv_map_.erase(victim_key.value());
    }
    // add failed
    if (!replacement_policy_->Add(key)) {
      return false;
    }
    kv_map_[key] = value;
  } else {
    // update failed
    if (!replacement_policy_->Access(key)) {
      return false;
    }
    it->second = value;
  }
  return true;
}

template <typename KeyType, typename ValueType>
auto CacheSystem<KeyType, ValueType>::Get(const KeyType &key) -> std::optional<ValueType> {
  auto it = kv_map_.find(key);
  // not found
  if (it == kv_map_.end()) {
    return std::nullopt;
  }

  // update failed
  if (!replacement_policy_->Access(key)) {
    return std::nullopt;
  }

  return it->second;
}

template <typename KeyType, typename ValueType>
auto CacheSystem<KeyType, ValueType>::Remove(const KeyType &key) -> bool {
  auto it = kv_map_.find(key);
  // not found
  if (it == kv_map_.end()) {
    return false;
  }

  // remove failed
  if (!replacement_policy_->Remove(key)) {
    return false;
  }

  kv_map_.erase(it);
  return true;
}

template class CacheSystem<int, int>;

}  // namespace utilities