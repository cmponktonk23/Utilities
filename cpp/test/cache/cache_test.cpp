#include <gtest/gtest.h>

#include "cache/cache.h"
#include "test_utils.h"

namespace utilities {

// TEST(CacheTest, LRUBasicTest) {
//   auto factory = std::make_unique<ReplacementPolicyFactory<int>>();
//   CacheSystem<int, int> lru_cache(std::move(factory), ReplacementPolicyEnum::LRU, 2);

//   lru_cache.Put(1, 1);
//   lru_cache.Put(2, 2);
//   EXPECT_EQ(lru_cache.Get(1), 1);
//   lru_cache.Put(3, 3);
//   EXPECT_THROW(lru_cache.Get(2), CacheException);
//   lru_cache.Put(4, 4);

//   try {
//     lru_cache.Get(1);
//     FAIL() << "Expected CacheException";
//   } catch (const CacheException &e) {
//     EXPECT_EQ(std::string(e.what()), "Key not found");
//   } catch (...) {
//     FAIL() << "Expected CacheException";
//   }

//   EXPECT_EQ(lru_cache.Get(3), 3);
//   EXPECT_EQ(lru_cache.Get(4), 4);
// }

// TEST(CacheTest, LRUKBasicTest) {
//   auto factory = std::make_unique<ReplacementPolicyFactory<int>>();
//   CacheSystem<int, int> lru_cache(std::move(factory), ReplacementPolicyEnum::LRU_K, 2);

//   lru_cache.Put(1, 1);
//   lru_cache.Put(2, 2);
//   EXPECT_EQ(lru_cache.Get(1), 1);
//   lru_cache.Put(3, 3);
//   EXPECT_THROW(lru_cache.Get(2), CacheException);
//   lru_cache.Put(4, 4);

//   try {
//     lru_cache.Get(1);
//     FAIL() << "Expected CacheException";
//   } catch (const CacheException &e) {
//     EXPECT_EQ(std::string(e.what()), "Key not found");
//   } catch (...) {
//     FAIL() << "Expected CacheException";
//   }

//   EXPECT_EQ(lru_cache.Get(3), 3);
//   EXPECT_EQ(lru_cache.Get(4), 4);
// }

TEST(CacheTest, LRUKBasicTest2) {
  auto factory = std::make_unique<ReplacementPolicyFactory<int>>();
  CacheSystem<int, int> lru_cache(std::move(factory), ReplacementPolicyEnum::LRU_K, 2);

  lru_cache.Put(2, 1);
  lru_cache.Put(2, 2);
  EXPECT_EQ(lru_cache.Get(2), 2);
  lru_cache.Put(1, 1);
  lru_cache.Put(4, 1);
  EXPECT_THROW(lru_cache.Get(2), CacheException);
}

// TEST(CacheTest, LFUBasicTest) {
//   auto factory = std::make_unique<ReplacementPolicyFactory<int>>();
//   CacheSystem<int, int> lfu_cache(std::move(factory), ReplacementPolicyEnum::LFU, 2);

//   lfu_cache.Put(1, 1);
//   lfu_cache.Put(2, 2);
//   EXPECT_EQ(lfu_cache.Get(1), 1);
//   lfu_cache.Put(3, 3);
//   EXPECT_THROW(lfu_cache.Get(2), CacheException);
//   EXPECT_EQ(lfu_cache.Get(3), 3);
//   lfu_cache.Put(4, 4);

//   try {
//     lfu_cache.Get(1);
//     FAIL() << "Expected CacheException";
//   } catch (const CacheException &e) {
//     EXPECT_EQ(std::string(e.what()), "Key not found");
//   } catch (...) {
//     FAIL() << "Expected CacheException";
//   }

//   EXPECT_EQ(lfu_cache.Get(3), 3);
//   EXPECT_EQ(lfu_cache.Get(4), 4);
// }

}  // namespace utilities
