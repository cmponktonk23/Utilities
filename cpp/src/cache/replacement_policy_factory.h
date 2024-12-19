#pragma once

#include <exception>
#include <memory>

#include "replacement_policy.h"

namespace utilities {

template <typename KeyType>
class IReplacementPolicyFactory {
 public:
  IReplacementPolicyFactory() = default;
  virtual ~IReplacementPolicyFactory() = default;

  // 1. Why we can't use template parameters on virtual function?
  // Because polymorphism is based on vtable, which is a runtime mechanism. However, template is a compile-time mechanism.
  // So, that's why we can only define the class as a template class.
  // 2. Use unique_ptr to manage the lifecycle of the factory product.
  virtual auto Create(const ReplacementPolicyEnum &policy) -> std::unique_ptr<IReplacementPolicy<KeyType>> = 0;
};

template <typename KeyType>
class ReplacementPolicyFactory : public IReplacementPolicyFactory<KeyType> {
 public:
  ReplacementPolicyFactory() = default;
  ~ReplacementPolicyFactory() override = default;

  auto Create(const ReplacementPolicyEnum &policy) -> std::unique_ptr<IReplacementPolicy<KeyType>> override;
};

}  // namespace utilities