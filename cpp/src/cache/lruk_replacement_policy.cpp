#include "replacement_policy.h"

namespace utilities {

template <typename KeyType>
void LRUKNode<KeyType>::Access(size_t timestamp) {
  size_t len = history_.size();
  assert(len <= k_);
  if (len == k_) {
    history_.pop_back();
  }
  history_.push_front(timestamp);
}

template <typename KeyType>
auto LRUKReplacementPolicy<KeyType>::Add(const KeyType &key) -> bool {
  // key already exists
  if (key_node_map_.find(key) != key_node_map_.end()) {
    return false;
  }
  key_node_map_[key] = std::make_shared<LRUKNode<KeyType>>(key, k_, timestamp_++);
  node_set_.insert(key_node_map_[key]);
  return true;
}

template <typename KeyType>
auto LRUKReplacementPolicy<KeyType>::Remove(const KeyType &key) -> bool {
  auto it = key_node_map_.find(key);
  // key not found
  if (it == key_node_map_.end()) {
    return false;
  }
  auto node = it->second;
  node_set_.erase(node);
  key_node_map_.erase(it);
  return true;
}

template <typename KeyType>
auto LRUKReplacementPolicy<KeyType>::Access(const KeyType &key) -> bool {
  auto it = key_node_map_.find(key);
  // key not found
  if (it == key_node_map_.end()) {
    return false;
  }
  auto node_it = it->second;
  auto node = it->second;
  node_set_.erase(node);
  node->Access(timestamp_++);
  node_set_.insert(node);
  return true;
}

template <typename KeyType>
auto LRUKReplacementPolicy<KeyType>::Evict() -> std::optional<KeyType> {
  // empty
  if (node_set_.empty()) {
    return std::nullopt;
  }
  auto node_it = node_set_.begin();
  auto victim_key = (*node_it)->key_;
  node_set_.erase(node_it);
  key_node_map_.erase(victim_key);
  return victim_key;
}

template class LRUKReplacementPolicy<int>;

}  // namespace utilities