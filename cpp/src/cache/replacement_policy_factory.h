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