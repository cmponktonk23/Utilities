using System.Threading.Tasks;

namespace CacheSystem
{
    /// <summary>
    /// Interface for cache replacement policies.
    /// </summary>
    public interface IReplacementPolicy<KeyType>
    {
        bool Add(KeyType key);
        bool Remove(KeyType key);
        bool Access(KeyType key);
        Task<KeyType> EvictAsync();
    }
}