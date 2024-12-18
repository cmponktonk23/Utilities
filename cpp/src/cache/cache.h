#pragma once  // prevent multiple inclusions of the header file

#include <memory>
#include <optional>
#include <unordered_map>

#include "replacement_policy_factory.h"

namespace utilities {

template <typename KeyType, typename ValueType>
class IKVCache {
 public:
  virtual ~IKVCache() = default;

  virtual auto Put(const KeyType &key, const ValueType &value) -> bool = 0;
  virtual auto Get(const KeyType &key) -> std::optional<ValueType> = 0;
  virtual auto Remove(const KeyType &key) -> bool = 0;
  //   virtual void pin(const KeyType& key) = 0;
  //   virtual void unpin(const KeyType& key) = 0;
};

template <typename KeyType, typename ValueType>
class CacheSystem : public IKVCache<KeyType, ValueType> {
 public:
  CacheSystem(std::unique_ptr<IReplacementPolicyFactory<KeyType>> replacement_policy_factory, size_t capacity)
      : replacement_policy_(std::move(replacement_policy_factory->Create(ReplacementPolicyEnum::LRU))),
        capacity_(capacity) {}

  ~CacheSystem() override = default;

  CacheSystem(const CacheSystem &other) = delete;
  auto operator=(const CacheSystem &other) -> CacheSystem & = delete;

  CacheSystem(CacheSystem &&other) = delete;
  auto operator=(CacheSystem &&other) -> CacheSystem & = delete;

  auto Put(const KeyType &key, const ValueType &value) -> bool override;
  auto Get(const KeyType &key) -> std::optional<ValueType> override;
  auto Remove(const KeyType &key) -> bool override;
  //   void pin(const KeyType& key) override;
  //   void unpin(const KeyType& key) override;

 private:
  std::unique_ptr<IReplacementPolicy<KeyType>> replacement_policy_;
  const size_t capacity_;
  std::unordered_map<KeyType, ValueType> kv_map_;
};

}  // namespace utilities