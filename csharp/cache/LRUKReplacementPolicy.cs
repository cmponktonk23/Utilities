using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace CacheSystem
{
    /// <summary>
    /// LRU-K replacement policy.
    /// </summary>
    public class LRUKReplacementPolicy<KeyType> : IReplacementPolicy<KeyType>
    {
        private class Node
        {
            public KeyType Key { get; }
            public Queue<long> AccessTimes { get; }

            public Node(KeyType key, int k)
            {
                Key = key;
                AccessTimes = new Queue<long>(k);
            }

            public void Access(long timestamp, int k)
            {
                if (AccessTimes.Count == k)
                {
                    AccessTimes.Dequeue();
                }
                AccessTimes.Enqueue(timestamp);
            }

            public long GetKthAccessTime()
            {
                return AccessTimes.Peek();
            }
        }

        private readonly int _k;
        private long _timestamp = 0;
        private readonly Dictionary<KeyType, Node> _keyNodeMap = new Dictionary<KeyType, Node>();
        private readonly SortedSet<(long, KeyType)> _nodeSet = new SortedSet<(long, KeyType)>();
        private readonly object _lock = new object();

        public LRUKReplacementPolicy(int k)
        {
            if (k <= 0)
            {
                throw new ArgumentException("K must be positive.", nameof(k));
            }
            _k = k;
        }

        public bool Add(KeyType key)
        {
            lock (_lock)
            {
                if (_keyNodeMap.ContainsKey(key))
                {
                    return false;
                }
                var node = new Node(key, _k);
                node.Access(_timestamp++, _k);
                _keyNodeMap[key] = node;
                _nodeSet.Add((node.GetKthAccessTime(), key));
                return true;
            }
        }

        public bool Remove(KeyType key)
        {
            lock (_lock)
            {
                if (!_keyNodeMap.TryGetValue(key, out var node))
                {
                    return false;
                }
                _nodeSet.Remove((node.GetKthAccessTime(), key));
                _keyNodeMap.Remove(key);
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
                _nodeSet.Remove((node.GetKthAccessTime(), key));
                node.Access(_timestamp++, _k);
                _nodeSet.Add((node.GetKthAccessTime(), key));
                return true;
            }
        }

        public async Task<KeyType> EvictAsync()
        {
            lock (_lock)
            {
                if (_nodeSet.Count == 0)
                {
                    throw new CacheException("No keys to evict.");
                }
                var (timestamp, key) = _nodeSet.Min;
                _nodeSet.Remove(_nodeSet.Min);
                _keyNodeMap.Remove(key);
                return key;
            }
        }
    }
}