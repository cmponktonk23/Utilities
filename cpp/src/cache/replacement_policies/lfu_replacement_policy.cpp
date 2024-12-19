#include <iostream>
#include "replacement_policy.h"

namespace utilities {

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::Add(const KeyType &key) -> bool {
  // key already exists
  if (key_node_map_.find(key) != key_node_map_.end()) {
    return false;
  }
  auto &lst = freq_list_map_[global_min_freq_];
  lst.emplace_front(key, 1);
  key_node_map_[key] = lst.begin();
  AddMinFreq();
  return true;
}

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::Remove(const KeyType &key) -> bool {
  auto it = key_node_map_.find(key);
  // key not found
  if (it == key_node_map_.end()) {
    return false;
  }
  auto lst_it = it->second;
  size_t freq = lst_it->second;
  auto &lst = freq_list_map_[freq];
  lst.erase(lst_it);
  key_node_map_.erase(it);
  if (lst.empty()) {
    freq_list_map_.erase(freq);
    RemoveFreq(freq);
  }
  return true;
}

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::Access(const KeyType &key) -> bool {
  auto it = key_node_map_.find(key);
  // key not found
  if (it == key_node_map_.end()) {
    return false;
  }
  auto lst_it = it->second;
  size_t freq = lst_it->second++;
  auto &src_lst = freq_list_map_[freq];
  auto &dst_lst = freq_list_map_[freq + 1];
  dst_lst.splice(dst_lst.begin(), src_lst, lst_it);

  if (src_lst.empty()) {
    freq_list_map_.erase(freq);
    IncreaseFreq(freq, true);
  } else {
    IncreaseFreq(freq, false);
  }
  return true;
}

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::Evict() -> std::optional<KeyType> {
  // empty
  if (key_node_map_.empty()) {
    return std::nullopt;
  }
  auto min_freq = GetMinFreq();
  assert(min_freq.has_value());
  auto it = freq_list_map_.find(min_freq.value());
  assert(it != freq_list_map_.end());
  std::list<std::pair<KeyType, size_t>> &lst = it->second;
  assert(!lst.empty());
  auto [victim_key, freq] = lst.back();
  lst.pop_back();
  key_node_map_.erase(victim_key);
  if (lst.empty()) {
    freq_list_map_.erase(it);
    RemoveFreq(freq);
  }
  return victim_key;
}

template <typename KeyType>
void LFUReplacementPolicy<KeyType>::AddMinFreq() {
  if (freq_map_.find(global_min_freq_) != freq_map_.end()) {
    return;
  }
  freq_list_.push_front(global_min_freq_);
  freq_map_[global_min_freq_] = freq_list_.begin();
}

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::GetMinFreq() -> std::optional<size_t> {
  if (freq_list_.empty()) {
    return std::nullopt;
  }
  return freq_list_.front();
}

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::IncreaseFreq(size_t freq, bool need_delete) -> bool {
  auto it = freq_map_.find(freq);
  if (it == freq_map_.end()) {
    return false;
  }
  auto &curr_it = it->second;
  if (freq_map_.find(freq + 1) == freq_map_.end()) {
    freq_map_[freq + 1] = freq_list_.insert(std::next(curr_it), freq + 1);
  }
  if (need_delete) {
    freq_list_.erase(curr_it);
    freq_map_.erase(it);
  }
  return true;
}

template <typename KeyType>
auto LFUReplacementPolicy<KeyType>::RemoveFreq(size_t freq) -> bool {
  auto it = freq_map_.find(freq);
  if (it == freq_map_.end()) {
    return false;
  }
  auto &curr_it = it->second;
  freq_list_.erase(curr_it);
  freq_map_.erase(it);
  return true;
}

template class LFUReplacementPolicy<int>;

}  // namespace utilities