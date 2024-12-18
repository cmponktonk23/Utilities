#include <gtest/gtest.h>

#include "cache/cache.h"
#include "test_utils.h"

namespace utilities {

TEST(CacheTest, LRUBasicTest) {
  auto factory = std::make_unique<ReplacementPolicyFactory<int>>();
  CacheSystem<int, int> lru_cache(std::move(factory), 2);

  lru_cache.Put(1, 1);
  lru_cache.Put(2, 2);
  EXPECT_EQ(lru_cache.Get(1), 1);
  lru_cache.Put(3, 3);
  EXPECT_EQ(lru_cache.Get(2), std::nullopt);
  lru_cache.Put(4, 4);
  EXPECT_EQ(lru_cache.Get(1), std::nullopt);
  EXPECT_EQ(lru_cache.Get(3), 3);
  EXPECT_EQ(lru_cache.Get(4), 4);
}

}  // namespace utilities
