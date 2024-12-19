namespace CacheSystem
{
    /// <summary>
    /// Interface for replacement policy factory.
    /// </summary>
    public interface IReplacementPolicyFactory<KeyType>
    {
        IReplacementPolicy<KeyType> Create(ReplacementPolicyEnum policy);
    }
}