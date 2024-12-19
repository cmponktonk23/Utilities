using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;

namespace CacheSystem
{
    /// <summary>
    /// Cache system implementing the key-value cache.
    /// </summary>
    public class CacheSystem<KeyType, ValueType> : IKVCache<KeyType, ValueType>
    {
        private readonly int _capacity;
        private readonly IReplacementPolicy<KeyType> _replacementPolicy;
        private readonly ConcurrentDictionary<KeyType, ValueType> _cache = new ConcurrentDictionary<KeyType, ValueType>();
        private readonly SemaphoreSlim _lock = new SemaphoreSlim(1, 1);

        public CacheSystem(int capacity, IReplacementPolicyFactory<KeyType> factory, ReplacementPolicyEnum policy)
        {
            _capacity = capacity;
            _replacementPolicy = factory.Create(policy);
        }

        public async Task<bool> PutAsync(KeyType key, ValueType value)
        {
            await _lock.WaitAsync();
            try
            {
                if (_cache.ContainsKey(key))
                {
                    _cache[key] = value;
                    if (!_replacementPolicy.Access(key))
                    {
                        throw new CacheException("Failed to update key in replacement policy.");
                    }
                }
                else
                {
                    if (_cache.Count >= _capacity)
                    {
                        var victimKey = await _replacementPolicy.EvictAsync();
                        if (!_cache.TryRemove(victimKey, out _))
                        {
                            throw new CacheException("Failed to evict key from cache.");
                        }
                    }
                    if (!_replacementPolicy.Add(key))
                    {
                        throw new CacheException("Failed to add key to replacement policy.");
                    }
                    if (!_cache.TryAdd(key, value))
                    {
                        throw new CacheException("Failed to add key to cache.");
                    }
                }
                return true;
            }
            finally
            {
                _lock.Release();
            }
        }

        public async Task<ValueType> GetAsync(KeyType key)
        {
            await _lock.WaitAsync();
            try
            {
                if (!_cache.TryGetValue(key, out var value))
                {
                    throw new CacheException("Key not found.");
                }
                if (!_replacementPolicy.Access(key))
                {
                    throw new CacheException("Failed to access key in replacement policy.");
                }
                return value;
            }
            finally
            {
                _lock.Release();
            }
        }

        public async Task<bool> RemoveAsync(KeyType key)
        {
            await _lock.WaitAsync();
            try
            {
                if (!_cache.TryRemove(key, out _))
                {
                    throw new CacheException("Key not found.");
                }
                if (!_replacementPolicy.Remove(key))
                {
                    throw new CacheException("Failed to remove key from replacement policy.");
                }
                return true;
            }
            finally
            {
                _lock.Release();
            }
        }
    }
}