using System;

namespace CacheSystem
{
    /// <summary>
    /// Custom exception for cache errors.
    /// </summary>
    public class CacheException : Exception
    {
        public CacheException(string message) : base(message)
        {
        }
    }
}