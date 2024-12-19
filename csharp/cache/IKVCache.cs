using System.Threading.Tasks;

namespace CacheSystem
{
    /// <summary>
    /// Interface for key-value cache.
    /// </summary>
    public interface IKVCache<KeyType, ValueType>
    {
        Task<bool> PutAsync(KeyType key, ValueType value);
        Task<ValueType> GetAsync(KeyType key);
        Task<bool> RemoveAsync(KeyType key);
    }
}