using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace CacheSystem
{
    /// <summary>
    /// Least Recently Used (LRU) cache replacement policy.
    /// </summary>
    public class LRUReplacementPolicy<KeyType> : IReplacementPolicy<KeyType>
    {
        private readonly LinkedList<KeyType> _lruList = new LinkedList<KeyType>();
        private readonly ConcurrentDictionary<KeyType, LinkedListNode<KeyType>> _keyNodeMap = new ConcurrentDictionary<KeyType, LinkedListNode<KeyType>>();
        private readonly object _lock = new object();

        public bool Add(KeyType key)
        {
            lock (_lock)
            {
                if (_keyNodeMap.ContainsKey(key))
                {
                    return false;
                }
                var node = new LinkedListNode<KeyType>(key);
                _lruList.AddFirst(node);
                _keyNodeMap[key] = node;
                return true;
            }
        }

        public bool Remove(KeyType key)
        {
            lock (_lock)
            {
                if (!_keyNodeMap.TryRemove(key, out var node))
                {
                    return false;
                }
                _lruList.Remove(node);
                return true;
            }
        }

        public bool Access(KeyType key)
        {
            lock (_lock)
            {
                if (!_keyNodeMap.TryGetValue(key, out var node))
                {
                    return false;
                }
                _lruList.Remove(node);
                _lruList.AddFirst(node);
                return true;
            }
        }

        public async Task<KeyType> EvictAsync()
        {
            lock (_lock)
            {
                if (_lruList.Count == 0)
                {
                    throw new CacheException("No keys to evict.");
                }
                var node = _lruList.Last;
                if (node == null)
                {
                    throw new CacheException("No keys to evict.");
                }
                _lruList.RemoveLast();
                _keyNodeMap.TryRemove(node.Value, out _);
                return node.Value;
            }
        }
    }
}