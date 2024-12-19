using System;

namespace CacheSystem
{
    /// <summary>
    /// Factory to create replacement policy instances.
    /// </summary>
    public class ReplacementPolicyFactory<KeyType> : IReplacementPolicyFactory<KeyType>
    {
        public IReplacementPolicy<KeyType> Create(ReplacementPolicyEnum policy)
        {
            switch (policy)
            {
                case ReplacementPolicyEnum.LRU:
                    return new LRUReplacementPolicy<KeyType>();
                case ReplacementPolicyEnum.LFU:
                    return new LFUReplacementPolicy<KeyType>();
                case ReplacementPolicyEnum.LRU_K:
                    return new LRUKReplacementPolicy<KeyType>(k: 2); // Assuming k=2
                default:
                    throw new CacheException("Unsupported replacement policy.");
            }
        }
    }
}