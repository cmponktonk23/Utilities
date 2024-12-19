#include "replacement_policy.h"

namespace utilities {

template <typename KeyType>
auto LRUReplacementPolicy<KeyType>::Add(const KeyType &key) -> bool {
  // key already exists
  if (key_node_map_.find(key) != key_node_map_.end()) {
    return false;
  }
  lru_list_.push_front(key);
  key_node_map_[key] = lru_list_.begin();
  return true;
}

template <typename KeyType>
auto LRUReplacementPolicy<KeyType>::Remove(const KeyType &key) -> bool {
  auto it = key_node_map_.find(key);
  // key not found
  if (it == key_node_map_.end()) {
    return false;
  }
  lru_list_.erase(it->second);
  key_node_map_.erase(it);
  return true;
}

template <typename KeyType>
auto LRUReplacementPolicy<KeyType>::Access(const KeyType &key) -> bool {
  auto it = key_node_map_.find(key);
  // key not found
  if (it == key_node_map_.end()) {
    return false;
  }
  lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
  return true;
}

template <typename KeyType>
auto LRUReplacementPolicy<KeyType>::Evict() -> std::optional<KeyType> {
  // empty
  if (lru_list_.empty()) {
    return std::nullopt;
  }
  auto victim_key = lru_list_.back();
  lru_list_.pop_back();
  key_node_map_.erase(victim_key);
  return victim_key;
}

template class LRUReplacementPolicy<int>;

}  // namespace utilities