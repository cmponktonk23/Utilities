#include "replacement_policy_factory.h"

#include <stdexcept>

namespace utilities {

template <typename KeyType>
auto ReplacementPolicyFactory<KeyType>::Create(const ReplacementPolicyEnum &policy)
    -> std::unique_ptr<IReplacementPolicy<KeyType>> {
  switch (policy) {
    case ReplacementPolicyEnum::LRU:
      return std::make_unique<LRUReplacementPolicy<KeyType>>();
    case ReplacementPolicyEnum::LFU:
      return std::make_unique<LFUReplacementPolicy<KeyType>>();
    case ReplacementPolicyEnum::LRU_K:
      return std::make_unique<LRUKReplacementPolicy<KeyType>>();
    default:
      throw std::invalid_argument("Unsupported replacement policy.");
  }
  return nullptr;
}

template class ReplacementPolicyFactory<int>;

}  // namespace utilities