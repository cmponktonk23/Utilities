#pragma once  // prevent multiple inclusions of the header file

#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>

#include "cache_exception.h"
#include "replacement_policies/replacement_policy_factory.h"

namespace utilities {

// 1. Abstract class as interface.
template <typename KeyType, typename ValueType>
class IKVCache {
 public:
  // 2. Virtual destructor for polymorphism.
  virtual ~IKVCache() = default;

  // 3. Pure virtual function to declare abstract class.
  // 4. Pass by reference to const
  virtual auto Put(const KeyType &key, const ValueType &value) -> bool = 0;
  virtual auto Get(const KeyType &key) -> std::optional<ValueType> = 0;
  virtual auto Remove(const KeyType &key) -> bool = 0;
  // virtual void pin(const KeyType& key) = 0;
  // virtual void unpin(const KeyType& key) = 0;
};

template <typename KeyType, typename ValueType>
// 5. Specify the base class template parameters during inheriting a template base class.
class CacheSystem : public IKVCache<KeyType, ValueType> {
 public:
  // 6. Abstract type can not be used as a value type, it can only be a pointer or reference type.
  // 7. unique_ptr<IFactory> due to factory can only have one instance.
  // 8. Dependency injection
  CacheSystem(std::unique_ptr<IReplacementPolicyFactory<KeyType>> replacement_policy_factory,
              ReplacementPolicyEnum policy_type, size_t capacity)
      : capacity_(capacity) {
    try {
      replacement_policy_ = std::move(replacement_policy_factory->Create(policy_type));
    } catch (const CacheException &e) {
      std::cerr << "Create replacement policy failed: " << e.what() << std::endl;
    }
  }

  // 9. override destructor
  ~CacheSystem() override = default;

  // 10. delete copy and move constructor and assignment
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
  std::mutex cache_mutex_;
};

}  // namespace utilities