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

        auto             old_capacity = a.capacity();
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

    // copy & move(with hole)
    {
        SparseArray<u32> a;
        a.append({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(4);
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_GE(a.capacity(), 6);
        ASSERT_EQ(a[0], 1);
        // ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        SparseArray<u32> b(a);
        ASSERT_EQ(b.size(), 4);
        ASSERT_EQ(b.sparseSize(), 6);
        ASSERT_EQ(b.holeSize(), 2);
        ASSERT_GE(b.capacity(), 6);
        ASSERT_EQ(b[0], 1);
        // ASSERT_EQ(b[1], 1);
        ASSERT_EQ(b[2], 4);
        ASSERT_EQ(b[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(b[5], 4);

        b.append({2, 2});
        ASSERT_EQ(b.size(), 6);
        ASSERT_EQ(b.sparseSize(), 6);
        ASSERT_EQ(b.holeSize(), 0);
        ASSERT_GE(b.capacity(), 6);
        ASSERT_EQ(b[0], 1);
        ASSERT_EQ(b[1], 2);
        ASSERT_EQ(b[2], 4);
        ASSERT_EQ(b[3], 5);
        ASSERT_EQ(b[4], 2);
        ASSERT_EQ(b[5], 4);

        auto             old_capacity = a.capacity();
        SparseArray<u32> c(std::move(a));
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 0);
        ASSERT_EQ(c.size(), 4);
        ASSERT_EQ(c.sparseSize(), 6);
        ASSERT_EQ(c.holeSize(), 2);
        ASSERT_EQ(c.capacity(), old_capacity);
        ASSERT_EQ(c[0], 1);
        // ASSERT_EQ(c[1], 1);
        ASSERT_EQ(c[2], 4);
        ASSERT_EQ(c[3], 5);
        // ASSERT_EQ(c[4], 1);
        ASSERT_EQ(c[5], 4);

        c.append({2, 2, 1});
        ASSERT_EQ(c.size(), 7);
        ASSERT_EQ(c.sparseSize(), 7);
        ASSERT_EQ(c.holeSize(), 0);
        ASSERT_GE(c.capacity(), 7);
        ASSERT_EQ(c[0], 1);
        ASSERT_EQ(c[1], 2);
        ASSERT_EQ(c[2], 4);
        ASSERT_EQ(c[3], 5);
        ASSERT_EQ(c[4], 2);
        ASSERT_EQ(c[5], 4);
        ASSERT_EQ(c[6], 1);
    }

    // assign & move assign
    {
        SparseArray<u32> a, b, c;
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

        b.append({114514, 114514, 1, 1, 4});
        b = a;
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

        c.append({1, 1, 4, 514, 514, 514});
        auto old_capacity = a.capacity();
        c = std::move(a);
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

    // assign & move assign(with hole)
    {
        SparseArray<u32> a, b, c;
        a.append({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(4);
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_GE(a.capacity(), 6);
        ASSERT_EQ(a[0], 1);
        // ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        b.append({114514, 114514, 1, 1, 4});
        b = a;
        ASSERT_EQ(b.size(), 4);
        ASSERT_EQ(b.sparseSize(), 6);
        ASSERT_EQ(b.holeSize(), 2);
        ASSERT_GE(b.capacity(), 6);
        ASSERT_EQ(b[0], 1);
        // ASSERT_EQ(b[1], 1);
        ASSERT_EQ(b[2], 4);
        ASSERT_EQ(b[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(b[5], 4);

        b.append({2, 2});
        ASSERT_EQ(b.size(), 6);
        ASSERT_EQ(b.sparseSize(), 6);
        ASSERT_EQ(b.holeSize(), 0);
        ASSERT_GE(b.capacity(), 6);
        ASSERT_EQ(b[0], 1);
        ASSERT_EQ(b[1], 2);
        ASSERT_EQ(b[2], 4);
        ASSERT_EQ(b[3], 5);
        ASSERT_EQ(b[4], 2);
        ASSERT_EQ(b[5], 4);

        c.append({1, 1, 4, 514, 514, 514});
        auto old_capacity = a.capacity();
        c = std::move(a);
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 0);
        ASSERT_EQ(c.size(), 4);
        ASSERT_EQ(c.sparseSize(), 6);
        ASSERT_EQ(c.holeSize(), 2);
        ASSERT_EQ(c.capacity(), old_capacity);
        ASSERT_EQ(c[0], 1);
        // ASSERT_EQ(c[1], 1);
        ASSERT_EQ(c[2], 4);
        ASSERT_EQ(c[3], 5);
        // ASSERT_EQ(c[4], 1);
        ASSERT_EQ(c[5], 4);

        c.append({2, 2, 1});
        ASSERT_EQ(c.size(), 7);
        ASSERT_EQ(c.sparseSize(), 7);
        ASSERT_EQ(c.holeSize(), 0);
        ASSERT_GE(c.capacity(), 7);
        ASSERT_EQ(c[0], 1);
        ASSERT_EQ(c[1], 2);
        ASSERT_EQ(c[2], 4);
        ASSERT_EQ(c[3], 5);
        ASSERT_EQ(c[4], 2);
        ASSERT_EQ(c[5], 4);
        ASSERT_EQ(c[6], 1);
    }

    // compare
    {
        SparseArray<u32> a, b, c;
        a.append({1, 1, 4, 5, 1, 4});
        b.append({114, 114, 514, 114, 514, 114});
        c.append({1, 1, 4, 5, 1, 4});

        ASSERT_EQ(a, c);
        ASSERT_NE(a, b);

        c.removeAt(1);
        ASSERT_NE(a, c);

        a.removeAt(1);
        ASSERT_EQ(a, c);
    }
}