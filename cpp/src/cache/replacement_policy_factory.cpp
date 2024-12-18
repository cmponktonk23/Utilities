#include "replacement_policy_factory.h"

#include <exception>

namespace utilities {

template <typename KeyType>
auto ReplacementPolicyFactory<KeyType>::Create(const ReplacementPolicyEnum &policy)
    -> std::unique_ptr<IReplacementPolicy<KeyType>> {
  switch (policy) {
    case ReplacementPolicyEnum::LRU:
      return std::make_unique<LRUReplacementPolicy<KeyType>>();
    // case ReplacementPolicyEnum::LFU:
    //   //   throw std::runtime_error("Not implement");
    //   return nullptr;
    default:
      return nullptr;
  }
  return nullptr;
}

template class ReplacementPolicyFactory<int>;

}  // namespace utilities