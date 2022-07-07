#include <gtest/gtest.h>
#include <kun/core/mimimal.h>
#include <iostream>

TEST(TestCore, test_sparse_array)
{
    using namespace kun;

    // ctor
    {
        SparseArray<u32> a;
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), 0);
    }

    // copy & move
    {
        SparseArray<u32> a;
        a.append({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(a.size(), 6);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 6);
    }
}