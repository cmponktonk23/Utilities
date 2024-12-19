using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace CacheSystem
{
    /// <summary>
    /// Least Frequently Used (LFU) cache replacement policy.
    /// </summary>
    public class LFUReplacementPolicy<KeyType> : IReplacementPolicy<KeyType>
    {
        private readonly Dictionary<KeyType, int> _keyFrequency = new Dictionary<KeyType, int>();
        private readonly SortedDictionary<int, HashSet<KeyType>> _frequencyKeys = new SortedDictionary<int, HashSet<KeyType>>();
        private readonly object _lock = new object();
        private int _minFrequency = 1;

        public bool Add(KeyType key)
        {
            lock (_lock)
            {
                if (_keyFrequency.ContainsKey(key))
                {
                    return false;
                }
                _keyFrequency[key] = 1;
                if (!_frequencyKeys.ContainsKey(1))
                {
                    _frequencyKeys[1] = new HashSet<KeyType>();
                }
                _frequencyKeys[1].Add(key);
                _minFrequency = 1;
                return true;
            }
        }

        public bool Remove(KeyType key)
        {
            lock (_lock)
            {
                if (!_keyFrequency.TryGetValue(key, out var freq))
                {
                    return false;
                }
                _keyFrequency.Remove(key);
                _frequencyKeys[freq].Remove(key);
                if (_frequencyKeys[freq].Count == 0)
                {
                    _frequencyKeys.Remove(freq);
                    if (_minFrequency == freq)
                    {
                        _minFrequency = _frequencyKeys.Keys.FirstOrDefault();
                    }
                }
                return true;
            }
        }

        public bool Access(KeyType key)
        {
            lock (_lock)
            {
                if (!_keyFrequency.TryGetValue(key, out var freq))
                {
                    return false;
                }
                _keyFrequency[key] = freq + 1;
                _frequencyKeys[freq].Remove(key);
                if (_frequencyKeys[freq].Count == 0)
                {
                    _frequencyKeys.Remove(freq);
                    if (_minFrequency == freq)
                    {
                        _minFrequency++;
                    }
                }
                if (!_frequencyKeys.ContainsKey(freq + 1))
                {
                    _frequencyKeys[freq + 1] = new HashSet<KeyType>();
                }
                _frequencyKeys[freq + 1].Add(key);
                return true;
            }
        }

        public async Task<KeyType> EvictAsync()
        {
            lock (_lock)
            {
                if (_frequencyKeys.Count == 0)
                {
                    throw new CacheException("No keys to evict.");
                }
                var keySet = _frequencyKeys[_minFrequency];
                var key = keySet.First();
                keySet.Remove(key);
                if (keySet.Count == 0)
                {
                    _frequencyKeys.Remove(_minFrequency);
                }
                _keyFrequency.Remove(key);
                return key;
            }
        }
    }
}