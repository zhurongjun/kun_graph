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
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        SparseArray<u32> b(a);
        ASSERT_EQ(b.size(), 6);
        ASSERT_EQ(b.sparseSize(), 6);
        ASSERT_EQ(b.holeSize(), 0);
        ASSERT_GE(b.capacity(), 6);
        ASSERT_EQ(b[0], 1);
        ASSERT_EQ(b[1], 1);
        ASSERT_EQ(b[2], 4);
        ASSERT_EQ(b[3], 5);
        ASSERT_EQ(b[4], 1);
        ASSERT_EQ(b[5], 4);

        auto old_capacity = a.capacity();
        SparseArray<u32> c(std::move(a));
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 0);
        ASSERT_EQ(c.size(), 6);
        ASSERT_EQ(c.sparseSize(), 6);
        ASSERT_EQ(c.holeSize(), 0);
        ASSERT_EQ(c.capacity(), old_capacity);
        ASSERT_EQ(c[0], 1);
        ASSERT_EQ(c[1], 1);
        ASSERT_EQ(c[2], 4);
        ASSERT_EQ(c[3], 5);
        ASSERT_EQ(c[4], 1);
        ASSERT_EQ(c[5], 4);
    }
}