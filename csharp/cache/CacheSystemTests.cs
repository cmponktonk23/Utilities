using System.Threading.Tasks;
using Xunit;

namespace CacheSystem.Tests
{
    public class CacheSystemTests
    {
        [Fact]
        public async Task LRUBasicTest()
        {
            var factory = new ReplacementPolicyFactory<int>();
            var cache = new CacheSystem<int, int>(2, factory, ReplacementPolicyEnum.LRU);

            Assert.True(await cache.PutAsync(1, 1));
            Assert.True(await cache.PutAsync(2, 2));
            Assert.Equal(1, await cache.GetAsync(1));
            Assert.True(await cache.PutAsync(3, 3));

            await Assert.ThrowsAsync<CacheException>(async () => await cache.GetAsync(2));

            Assert.True(await cache.PutAsync(4, 4));
            await Assert.ThrowsAsync<CacheException>(async () => await cache.GetAsync(1));

            Assert.Equal(3, await cache.GetAsync(3));
            Assert.Equal(4, await cache.GetAsync(4));
        }

        [Fact]
        public async Task LFUBasicTest()
        {
            var factory = new ReplacementPolicyFactory<int>();
            var cache = new CacheSystem<int, int>(2, factory, ReplacementPolicyEnum.LFU);

            Assert.True(await cache.PutAsync(1, 1));
            Assert.True(await cache.PutAsync(2, 2));
            Assert.Equal(1, await cache.GetAsync(1));
            Assert.True(await cache.PutAsync(3, 3));

            await Assert.ThrowsAsync<CacheException>(async () => await cache.GetAsync(2));

            Assert.Equal(3, await cache.GetAsync(3));
            Assert.True(await cache.PutAsync(4, 4));

            await Assert.ThrowsAsync<CacheException>(async () => await cache.GetAsync(1));

            Assert.Equal(3, await cache.GetAsync(3));
            Assert.Equal(4, await cache.GetAsync(4));
        }

        [Fact]
        public async Task LRUKBasicTest()
        {
            var factory = new ReplacementPolicyFactory<int>();
            var cache = new CacheSystem<int, int>(2, factory, ReplacementPolicyEnum.LRU_K);

            Assert.True(await cache.PutAsync(1, 1));
            Assert.True(await cache.PutAsync(2, 2));
            Assert.Equal(1, await cache.GetAsync(1));
            Assert.True(await cache.PutAsync(3, 3));

            await Assert.ThrowsAsync<CacheException>(async () => await cache.GetAsync(2));

            Assert.True(await cache.PutAsync(4, 4));
            await Assert.ThrowsAsync<CacheException>(async () => await cache.GetAsync(1));

            Assert.Equal(3, await cache.GetAsync(3));
            Assert.Equal(4, await cache.GetAsync(4));
        }
    }
}