#include <gtest/gtest.h>
#include <kun/core/mimimal.h>

TEST(TestCore, test_bit_array)
{
    using kun::BitArray;

    // test ctor
    {
        BitArray a;
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);
        ASSERT_TRUE(a.empty());

        BitArray b(20, true);
        ASSERT_EQ(b.size(), 20);
        ASSERT_GE(b.capacity(), 20);
        ASSERT_NE(b.data(), nullptr);
        for (int i = 0; i < 20; ++i) { ASSERT_EQ(b[i], true); }

        BitArray c(100, false);
        ASSERT_EQ(c.size(), 100);
        ASSERT_GE(c.capacity(), 100);
        ASSERT_NE(c.data(), nullptr);
        for (int i = 0; i < 100; ++i) { ASSERT_EQ(c[i], false); }
    }

    // test copy & move
    {
        BitArray a(30, true);
        for (int i = 0; i < 30; ++i) { a[i] = i % 2; }

        BitArray b(a);
        ASSERT_EQ(a.size(), b.size());
        ASSERT_EQ(a.capacity(), b.capacity());
        for (int i = 0; i < 30; ++i) { ASSERT_EQ(a[i], b[i]); }

        auto     old_data = a.data();
        BitArray c(std::move(a));
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);
        ASSERT_EQ(c.size(), b.size());
        ASSERT_EQ(c.capacity(), b.capacity());
        ASSERT_EQ(c.data(), old_data);
        for (int i = 0; i < 30; ++i) { ASSERT_EQ(b[i], c[i]); }
    }

    // test copy assign & move assign
    {
        BitArray a(30, true), b, c;

        b = a;
        ASSERT_EQ(a.size(), b.size());
        ASSERT_EQ(a.capacity(), b.capacity());
        for (int i = 0; i < 30; ++i) { ASSERT_EQ(a[i], b[i]); }

        auto old_data = a.data();
        c = std::move(a);
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);
        ASSERT_EQ(c.size(), b.size());
        ASSERT_EQ(c.capacity(), b.capacity());
        ASSERT_EQ(c.data(), old_data);
        for (int i = 0; i < 30; ++i) { ASSERT_EQ(b[i], c[i]); }
    }

    // test compare
    {
        BitArray a(30, true), b(30, true), c(20, true);

        ASSERT_EQ(a, b);
        ASSERT_NE(a, c);
    }

    // [needn't test] getter

    // test validate
    {
        BitArray a(30, true);
        ASSERT_TRUE(a.isValidIndex(0));
        ASSERT_TRUE(a.isValidIndex(15));
        ASSERT_TRUE(a.isValidIndex(29));
        ASSERT_FALSE(a.isValidIndex(-1));
        ASSERT_FALSE(a.isValidIndex(39));
    }

    // test memory op
    {
        BitArray a(30, true);
        auto     old_capacity = a.capacity();
        a.clear();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), old_capacity);
        ASSERT_NE(a.data(), nullptr);

        a.release();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);

        a.reserve(40);
        ASSERT_EQ(a.size(), 0);
        ASSERT_GE(a.capacity(), 40);
        ASSERT_NE(a.data(), nullptr);

        a.resizeUnsafe(10);
        ASSERT_EQ(a.size(), 10);

        a.clear();
        a.resize(20, true);
        ASSERT_EQ(a.size(), 20);
        for (int i = 0; i < 20; ++i) { ASSERT_EQ(a[i], true); }

        a.resizeUnsafe(512);
        ASSERT_GE(a.capacity(), 512);

        a.reserve(4096);
        ASSERT_GE(a.capacity(), 4096);

        a.release(64);
        ASSERT_GE(a.capacity(), 64);

        a.resizeUnsafe(64);
        a.resizeUnsafe(16);
        ASSERT_EQ(a.size(), 16);
    }

    // test add
    {
        BitArray a;

        for (int i = 0; i < 40; ++i) { a.add(i % 3 == 0); }
        ASSERT_EQ(a.size(), 40);
        ASSERT_GE(a.capacity(), 40);
        for (int i = 0; i < 40; ++i) { ASSERT_EQ(a[i], i % 3 == 0); }

        a.add(true, 10);
        ASSERT_EQ(a.size(), 50);
        ASSERT_GE(a.capacity(), 50);
        for (int i = 40; i < 50; ++i) { ASSERT_EQ(a[i], true); }
    }

    // test remove
    {
        BitArray a;
        a.add(true, 10);
        a.add(false, 20);
        a.add(true, 10);
        a.removeAt(10, 20);
        ASSERT_EQ(a.size(), 20);
        for (int i = 0; i < 20; ++i) { ASSERT_EQ(a[i], true); }

        a.add(false, 15);
        a.removeAtSwap(15, 5);
        ASSERT_EQ(a.size(), 30);
        for (int i = 0; i < 15; ++i) { ASSERT_EQ(a[i], true); }
        for (int i = 15; i < 30; ++i) { ASSERT_EQ(a[i], false); }

        a.add(true, 5);
        a.removeAtSwap(20, 10);
        ASSERT_EQ(a.size(), 25);
        for (int i = 15; i < 20; ++i) { ASSERT_EQ(a[i], false); }
        for (int i = 20; i < 25; ++i) { ASSERT_EQ(a[i], true); }
    }

    // test modify
    {
        BitArray a(30, true);

        for (int i = 0; i < 30; ++i)
        {
            if (i % 3 == 0)
            {
                a[i] = false;
            }
        }
        ASSERT_EQ(a.size(), 30);

        for (int i = 0; i < 30; ++i) { ASSERT_EQ(a[i], i % 3 != 0); }
        ASSERT_EQ(a.size(), 30);
        for (int i = 0; i < 30; ++i)
        {
            if (i % 3 == 0)
            {
                a[i] = true;
            }
        }
        ASSERT_EQ(a.size(), 30);
        for (auto v : a) { ASSERT_EQ(v, true); }
        ASSERT_EQ(a.size(), 30);
        for (auto v : a) { v = false; }
        ASSERT_EQ(a.size(), 30);
        for (bool v : a) { ASSERT_EQ(v, false); }
        ASSERT_EQ(a.size(), 30);
    }

    // test find
    {
        BitArray a(30, true), b(30, false);
        a[20] = a[10] = false;
        b[20] = b[10] = true;

        ASSERT_EQ(a.find(false), 10);
        ASSERT_EQ(a.findLast(false), 20);
        ASSERT_EQ(b.find(true), 10);
        ASSERT_EQ(b.findLast(true), 20);
    }

    // [included in above tests] setRange()

    // test true it
    {
        BitArray a;
        a.reserve(30);
        for (int i = 0; i < 30; ++i) { a.add(i % 5 == 0); }

        for (BitArray<>::TIt it(a); it; ++it) { ASSERT_TRUE(it.index() % 5 == 0); }
    }
}