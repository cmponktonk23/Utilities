#pragma once

#include <cassert>
#include <list>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>

namespace utilities {

enum class ReplacementPolicyEnum {
  LRU,
  LFU,
  LRU_K,
};

template <typename KeyType>
class IReplacementPolicy {
 public:
  IReplacementPolicy() = default;
  virtual ~IReplacementPolicy() = default;

  virtual auto Add(const KeyType &key) -> bool = 0;
  virtual auto Remove(const KeyType &key) -> bool = 0;
  virtual auto Access(const KeyType &key) -> bool = 0;
  virtual auto Evict() -> std::optional<KeyType> = 0;
  // virtual void pin() = 0;
  // virtual void unpin() = 0;
};

template <typename KeyType>
class LRUReplacementPolicy : public IReplacementPolicy<KeyType> {
 public:
  LRUReplacementPolicy() = default;
  ~LRUReplacementPolicy() override = default;

  auto Add(const KeyType &key) -> bool override;
  auto Remove(const KeyType &key) -> bool override;
  auto Access(const KeyType &key) -> bool override;
  auto Evict() -> std::optional<KeyType> override;
  // bool pin() override;
  // void unpin() override;

 private:
  std::list<KeyType> lru_list_;

  // Why we choose to use value type of iterator here?
  // 1. list iterators are stable: not invalidated by insertions or deletions of other elements.
  // 2. Since list iterators are lightweight and copyable, storing by value is acceptable and efficient.
  // 3. Storing an iterator by reference is unsafe because the original iterator might go out of scope or change,
  // leading to dangling references.
  // 4. Smart Pointer is not applicable for iterators.

  // Why we use typename here?
  // When you have a template class, some of the types used within the class may depend on the template parameters.
  // These are called dependent names. The compiler cannot determine whether a dependent name is a type, a static
  // member, or a function without additional information. To resolve this ambiguity, you use the typename keyword to
  // explicitly specify that a dependent name is a type.
  std::unordered_map<KeyType, typename std::list<KeyType>::iterator> key_node_map_;
};

template <typename KeyType>
class LRUKReplacementPolicy;

template <typename KeyType>
class LRUKNode {
  friend class LRUKReplacementPolicy<KeyType>;

 public:
  explicit LRUKNode(const KeyType &key, size_t k, size_t timestamp) : key_(key), k_(k) { Access(timestamp); }

  void Access(size_t timestamp);

  struct Cmp {
    auto operator()(const std::shared_ptr<LRUKNode<KeyType>> &lhs, const std::shared_ptr<LRUKNode<KeyType>> &rhs) const
        -> bool {
      return lhs->history_.size() < rhs->history_.size() ||
             (lhs->history_.size() == rhs->history_.size() && lhs->history_.back() < rhs->history_.back());
    }
  };

 private:
  KeyType key_;
  const size_t k_;
  std::list<size_t> history_;
};

template <typename KeyType>
class LRUKReplacementPolicy : public IReplacementPolicy<KeyType> {
 public:
  LRUKReplacementPolicy() = default;
  ~LRUKReplacementPolicy() override = default;

  auto Add(const KeyType &key) -> bool override;
  auto Remove(const KeyType &key) -> bool override;
  auto Access(const KeyType &key) -> bool override;
  auto Evict() -> std::optional<KeyType> override;

 private:
  const size_t k_{1};
  size_t timestamp_{0};
  std::unordered_map<KeyType, std::shared_ptr<LRUKNode<KeyType>>> key_node_map_;
  std::set<std::shared_ptr<LRUKNode<KeyType>>, typename LRUKNode<KeyType>::Cmp> node_set_;
};

template <typename KeyType>
class LFUReplacementPolicy : public IReplacementPolicy<KeyType> {
 public:
  LFUReplacementPolicy() = default;
  ~LFUReplacementPolicy() override = default;

  auto Add(const KeyType &key) -> bool override;
  auto Remove(const KeyType &key) -> bool override;
  auto Access(const KeyType &key) -> bool override;
  auto Evict() -> std::optional<KeyType> override;
  // bool pin() override;
  // void unpin() override;

 private:
  auto GetMinFreq() -> std::optional<size_t>;
  void AddMinFreq();
  auto IncreaseFreq(size_t freq, bool need_delete) -> bool;
  auto RemoveFreq(size_t freq) -> bool;

  std::unordered_map<size_t, std::list<std::pair<KeyType, size_t>>> freq_list_map_;
  std::unordered_map<KeyType, typename std::list<std::pair<KeyType, size_t>>::iterator> key_node_map_;
  std::unordered_map<size_t, std::list<size_t>::iterator> freq_map_;
  std::list<size_t> freq_list_;

  const size_t global_min_freq_ = 1;
};

}  // namespace utilities