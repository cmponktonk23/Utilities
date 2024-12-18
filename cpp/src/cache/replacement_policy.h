#pragma once

#include <list>
#include <optional>
#include <unordered_map>

namespace utilities {

/* 
  What's the difference between enum and enum class?
  
 */
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
  //   virtual void pin() = 0;
  //   virtual void unpin() = 0;
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
  //   bool pin() override;
  //   void unpin() override;

 private:
  std::list<KeyType> lru_list_;
  std::unordered_map<KeyType, typename std::list<KeyType>::iterator> node_map_;
};

}  // namespace utilities