#include <gtest/gtest.h>
#include <kun/core/mimimal.h>

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

        SparseArray<u32> b(100);
        ASSERT_EQ(b.size(), 100);
        ASSERT_EQ(b.sparseSize(), 100);
        ASSERT_EQ(b.holeSize(), 0);
        ASSERT_GE(b.capacity(), 100);
        for (Size i = 0; i < 100; ++i) { ASSERT_TRUE(b.hasData(i)); }

        SparseArray<u32> c(100, 114514);
        ASSERT_EQ(c.size(), 100);
        ASSERT_EQ(c.sparseSize(), 100);
        ASSERT_EQ(c.holeSize(), 0);
        ASSERT_GE(c.capacity(), 100);
        for (Size i = 0; i < 100; ++i)
        {
            ASSERT_TRUE(c.hasData(i));
            ASSERT_EQ(c[i], 114514);
        }

        SparseArray<u32> d({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(d.size(), 6);
        ASSERT_EQ(d.sparseSize(), 6);
        ASSERT_EQ(d.holeSize(), 0);
        ASSERT_GE(d.capacity(), 6);
        ASSERT_TRUE(d.hasData(0));
        ASSERT_EQ(d[0], 1);
        ASSERT_TRUE(d.hasData(1));
        ASSERT_EQ(d[1], 1);
        ASSERT_TRUE(d.hasData(2));
        ASSERT_EQ(d[2], 4);
        ASSERT_TRUE(d.hasData(3));
        ASSERT_EQ(d[3], 5);
        ASSERT_TRUE(d.hasData(4));
        ASSERT_EQ(d[4], 1);
        ASSERT_TRUE(d.hasData(5));
        ASSERT_EQ(d[5], 4);

        u32              data[] = {1, 1, 4, 5, 1, 4};
        SparseArray<u32> e(data, 6);
        ASSERT_EQ(e.size(), 6);
        ASSERT_EQ(e.sparseSize(), 6);
        ASSERT_EQ(e.holeSize(), 0);
        ASSERT_GE(e.capacity(), 6);
        ASSERT_TRUE(e.hasData(0));
        ASSERT_EQ(e[0], 1);
        ASSERT_TRUE(e.hasData(1));
        ASSERT_EQ(e[1], 1);
        ASSERT_TRUE(e.hasData(2));
        ASSERT_EQ(e[2], 4);
        ASSERT_TRUE(e.hasData(3));
        ASSERT_EQ(e[3], 5);
        ASSERT_TRUE(e.hasData(4));
        ASSERT_EQ(e[4], 1);
        ASSERT_TRUE(e.hasData(5));
        ASSERT_EQ(e[5], 4);
    }

    // copy & move
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
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
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
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
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        SparseArray<u32> b({114514, 114514, 1, 1, 4});
        SparseArray<u32> c({1, 1, 4, 514, 514, 514});
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
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        SparseArray<u32> b({114514, 114514, 1, 1, 4});
        SparseArray<u32> c({1, 1, 4, 514, 514, 514});
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

    // special assign
    {
        u32 data[100];
        for (Size i = 0; i < 100; ++i) { data[i] = 99 - i; }

        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(4);
        a.assign(data, 50);
        ASSERT_EQ(a.size(), 50);
        ASSERT_EQ(a.sparseSize(), 50);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 50);
        for (Size i = 0; i < 50; ++i) { a[i] = 99 - i; }

        a.assign({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(a.size(), 6);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 50);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);
    }

    // compare
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        SparseArray<u32> b({114, 114, 514, 114, 514, 114});
        SparseArray<u32> c({1, 1, 4, 5, 1, 4});

        ASSERT_EQ(a, c);
        ASSERT_NE(a, b);

        c.removeAt(1);
        ASSERT_NE(a, c);

        a.removeAt(1);
        ASSERT_EQ(a, c);
    }

    // [needn't test] getter

    // validate
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        a.removeAt(0);
        a.removeAt(2);
        a.removeAt(4);
        for (Size i = 0; i < 6; ++i) { ASSERT_NE(a.hasData(i), a.isHole(i)); }
        ASSERT_TRUE(a.isHole(0));
        ASSERT_TRUE(a.hasData(1));
        ASSERT_TRUE(a.isHole(2));
        ASSERT_TRUE(a.hasData(3));
        ASSERT_TRUE(a.isHole(4));
        ASSERT_TRUE(a.hasData(5));

        ASSERT_FALSE(a.isValidIndex(-1));
        ASSERT_TRUE(a.isValidIndex(0));
        ASSERT_TRUE(a.isValidIndex(3));
        ASSERT_TRUE(a.isValidIndex(5));
        ASSERT_FALSE(a.isValidIndex(6));

        ASSERT_FALSE(a.isValidPointer(&a[0] - 1));
        ASSERT_TRUE(a.isValidPointer(&a[0]));
        ASSERT_TRUE(a.isValidPointer(&a[3]));
        ASSERT_TRUE(a.isValidPointer(&a[5]));
        ASSERT_FALSE(a.isValidPointer(&a[5] + 4));
    }

    // memory op
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(5);
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_GE(a.capacity(), 6);

        auto old_capacity = a.capacity();
        a.clear();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), old_capacity);

        a.release();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), 0);

        a.release(5);
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.sparseSize(), 0);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), 5);

        a.reserve(100);
        a.append({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(a.size(), 6);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), 100);

        a.removeAt(4);
        a.removeAt(5);
        a.shrink();
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_EQ(a.capacity(), 6);

        a.clear();
        a.append({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(3);
        a.compact();
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 4);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 6);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 4);

        a.clear();
        a.append({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(3);
        a.compactStable();
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 4);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 6);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 4);
        ASSERT_EQ(a[2], 1);
        ASSERT_EQ(a[3], 4);
    }

    // add
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        auto             info = a.add(10);
        *info.data = 100;
        ASSERT_EQ(a.size(), 7);
        ASSERT_EQ(a.sparseSize(), 7);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 7);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);
        ASSERT_EQ(a[6], 100);

        a.removeAt(1);
        a.addZeroed();
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 0);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);
        ASSERT_EQ(a[6], 100);

        a.removeAt(1);
        a.removeAt(4);
        info = a.addUnsafe();
        *info.data = 114514;
        ASSERT_EQ(a[0], 1);
        // ASSERT_EQ(a[1], 0);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 114514);
        ASSERT_EQ(a[5], 4);
        ASSERT_EQ(a[6], 100);
    }

    // add at
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(4);
        a.addAt(1, 114514);
        a.addAt(4, 10086);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 10086);
        ASSERT_EQ(a[5], 4);

        a.removeAt(2);
        a.removeAt(5);
        a.addAtZeroed(2);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 0);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 10086);
        // ASSERT_EQ(a[5], 4);
    }

    // emplace
    {
        SparseArray<u32> a({{1, 1, 4, 5, 1, 4}});
        a.removeAt(1);
        a.removeAt(4);
        a.emplace(114514);
        a.emplaceAt(1, 10086);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 10086);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 114514);
        ASSERT_EQ(a[5], 4);
    }

    // append
    {
        SparseArray<u32> a;
        a.append({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(4);
        a.append({114514, 114514, 114514});
        ASSERT_EQ(a.size(), 7);
        ASSERT_EQ(a.sparseSize(), 7);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 7);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 114514);
        ASSERT_EQ(a[5], 4);
        ASSERT_EQ(a[6], 114514);

        SparseArray<u32> b;
        b.append({1, 1, 4, 5, 1, 4});
        b.removeAt(0);
        b.removeAt(1);
        b.removeAt(5);
        a.append(b);
        ASSERT_EQ(a.size(), 10);
        ASSERT_EQ(a.sparseSize(), 10);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 10);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 114514);
        ASSERT_EQ(a[5], 4);
        ASSERT_EQ(a[6], 114514);
        ASSERT_EQ(a[7], 4);
        ASSERT_EQ(a[8], 5);
        ASSERT_EQ(a[9], 1);

        Array<u32> c(100, 114514);
        a.removeAt(6, 4);
        a.append(c.data(), c.size());
        ASSERT_EQ(a.size(), 106);
        ASSERT_EQ(a.sparseSize(), 106);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_GE(a.capacity(), 106);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 114514);
        ASSERT_EQ(a[5], 4);
        for (Size i = 6; i < 106; ++i) { ASSERT_EQ(a[i], 114514); }
    }

    // remove
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        a.removeAt(1);
        a.removeAt(4);
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_TRUE(a.hasData(0));
        ASSERT_TRUE(a.isHole(1));
        ASSERT_TRUE(a.hasData(2));
        ASSERT_TRUE(a.hasData(3));
        ASSERT_TRUE(a.isHole(4));
        ASSERT_TRUE(a.hasData(5));
        ASSERT_EQ(a[0], 1);
        // ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        a.remove(4);
        ASSERT_EQ(a.size(), 3);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 3);
        ASSERT_TRUE(a.hasData(0));
        ASSERT_TRUE(a.isHole(1));
        ASSERT_TRUE(a.isHole(2));
        ASSERT_TRUE(a.hasData(3));
        ASSERT_TRUE(a.isHole(4));
        ASSERT_TRUE(a.hasData(5));
        ASSERT_EQ(a[0], 1);
        // ASSERT_EQ(a[1], 1);
        // ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        a.clear();
        a.append({1, 1, 4, 5, 1, 4});
        a.removeAt(3);
        a.removeLast(1);
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_TRUE(a.hasData(0));
        ASSERT_TRUE(a.hasData(1));
        ASSERT_TRUE(a.hasData(2));
        ASSERT_TRUE(a.isHole(3));
        ASSERT_TRUE(a.isHole(4));
        ASSERT_TRUE(a.hasData(5));
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        // ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        a.add(4);
        a.add(4);
        a.removeAll(4);
        ASSERT_EQ(a.size(), 2);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 4);
        ASSERT_TRUE(a.hasData(0));
        ASSERT_TRUE(a.hasData(1));
        ASSERT_TRUE(a.isHole(2));
        ASSERT_TRUE(a.isHole(3));
        ASSERT_TRUE(a.isHole(4));
        ASSERT_TRUE(a.isHole(5));
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        // ASSERT_EQ(a[2], 4);
        // ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        // ASSERT_EQ(a[5], 4);
    }

    // remove if
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});
        a.removeIf([](const u32& a) { return a > 3; });
        ASSERT_EQ(a.size(), 5);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 1);
        ASSERT_TRUE(a.hasData(0));
        ASSERT_TRUE(a.hasData(1));
        ASSERT_TRUE(a.isHole(2));
        ASSERT_TRUE(a.hasData(3));
        ASSERT_TRUE(a.hasData(4));
        ASSERT_TRUE(a.hasData(5));
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        // ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        a.removeLastIf([](const u32& a) { return a > 3; });
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 2);
        ASSERT_TRUE(a.hasData(0));
        ASSERT_TRUE(a.hasData(1));
        ASSERT_TRUE(a.isHole(2));
        ASSERT_TRUE(a.hasData(3));
        ASSERT_TRUE(a.hasData(4));
        ASSERT_TRUE(a.isHole(5));
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        // ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        // ASSERT_EQ(a[5], 4);

        a.removeAllIf([](const u32& a) { return a < 3; });
        ASSERT_EQ(a.size(), 1);
        ASSERT_EQ(a.sparseSize(), 6);
        ASSERT_EQ(a.holeSize(), 5);
        ASSERT_TRUE(a.isHole(0));
        ASSERT_TRUE(a.isHole(1));
        ASSERT_TRUE(a.isHole(2));
        ASSERT_TRUE(a.hasData(3));
        ASSERT_TRUE(a.isHole(4));
        ASSERT_TRUE(a.isHole(5));
        // ASSERT_EQ(a[0], 1);
        // ASSERT_EQ(a[1], 1);
        // ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        // ASSERT_EQ(a[4], 1);
        // ASSERT_EQ(a[5], 4);
    }

    // [needn't test] modify

    // [test in remove] find

    // [test in remove] find if

    // contain
    {
        SparseArray<u32> a({1, 1, 4, 5, 1, 4});

        ASSERT_TRUE(a.contain(5));
        a.removeAll(5);
        ASSERT_FALSE(a.contain(5));

        auto cond = [](const u32& a) { return a < 4; };
        ASSERT_TRUE(a.containIf(cond));
        a.removeAllIf(cond);
        ASSERT_FALSE(a.containIf(cond));
    }

    // sort
    {
        SparseArray<u32> a(1000);
        for (Size i = 0; i < 1000; ++i) { a[i] = 999 - i; }
        a.removeAllIf([](const u32& n) { return n % 2 == 1; });
        ASSERT_EQ(a.size(), 500);
        ASSERT_EQ(a.sparseSize(), 1000);
        ASSERT_EQ(a.holeSize(), 500);
        ASSERT_EQ(a.capacity(), 1000);
        a.sort();
        ASSERT_EQ(a.size(), 500);
        ASSERT_EQ(a.sparseSize(), 500);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), 1000);
        for (Size i = 0; i < 500; ++i) { ASSERT_EQ(a[i], i * 2); }

        a.clear();
        for (Size i = 0; i < 1000; ++i) { a.add(i); }
        a.removeAllIf([](const u32& n) { return n % 2 == 1; });
        ASSERT_EQ(a.size(), 500);
        ASSERT_EQ(a.sparseSize(), 1000);
        ASSERT_EQ(a.holeSize(), 500);
        ASSERT_EQ(a.capacity(), 1000);
        a.sortStable(Greater<u32>());
        ASSERT_EQ(a.size(), 500);
        ASSERT_EQ(a.sparseSize(), 500);
        ASSERT_EQ(a.holeSize(), 0);
        ASSERT_EQ(a.capacity(), 1000);
        for (Size i = 0; i < 500; ++i) { ASSERT_EQ(a[i], (499 - i) * 2); }
    }

    // foreach
    {
        SparseArray<u32> a(1000);
        for (Size i = 0; i < 1000; ++i) { a[i] = 999 - i; }

        Size count = 0;
        for (u32 v : a)
        {
            ASSERT_LT(v, 1000);
            ASSERT_GE(v, 0);
            ASSERT_EQ(v, 999 - count);
            ++count;
        }
        ASSERT_EQ(count, 1000);

        const auto& b = a;
        count = 0;
        for (u32 v : a)
        {
            ASSERT_LT(v, 1000);
            ASSERT_GE(v, 0);
            ASSERT_EQ(v, 999 - count);
            ++count;
        }
        ASSERT_EQ(count, 1000);
    }
}