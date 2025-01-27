#include <gtest/gtest.h>
#include "hyperloglog.h"
#include <set>
#include <random>

class HyperLogLogTest : public ::testing::Test {
protected:
    HyperLogLogTest() {}
    ~HyperLogLogTest() override {}
    void SetUp() override {}
    void TearDown() override {}
};

TEST(HyperLogLogTest, BasicInitialization) {
    HyperLogLog hll(4);  // 2^4 = 16 registers
    EXPECT_NEAR(hll.GetCardinality(), 0.0, 0.1);
}

TEST(HyperLogLogTest, SimpleCardinality) {
    HyperLogLog hll(4);
    
    // Add some numbers
    for (int i = 1; i <= 100; i++) {
        hll.AddElem(i);
    }
    
    // Since we added exactly 100 distinct elements
    // The estimate should be reasonably close
    double estimate = hll.GetCardinality();
    EXPECT_NEAR(estimate, 100.0, 100.0 * 0.2);  // Allow 20% error margin
}

TEST(HyperLogLogTest, DuplicateElements) {
    HyperLogLog hll(4);
    
    // Add same number multiple times
    for (int i = 0; i < 100; i++) {
        hll.AddElem(42);
    }
    
    double estimate = hll.GetCardinality();
    EXPECT_NEAR(estimate, 1.0, 1.0);  // Should be close to 1
}

TEST(HyperLogLogTest, RandomDistribution) {
    HyperLogLog hll(6);  // More registers for better accuracy
    std::set<uint64_t> actual_elements;
    
    // Generate random numbers
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    for (int i = 0; i < 10000; i++) {
        uint64_t val = dis(gen);
        hll.AddElem(val);
        actual_elements.insert(val);
    }
    
    double estimate = hll.GetCardinality();
    double actual = actual_elements.size();
    EXPECT_NEAR(estimate, actual, actual * 0.2);  // Allow 20% error margin
}
