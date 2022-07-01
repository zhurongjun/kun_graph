#include <gtest/gtest.h>
#include <kun/core/mimimal.h>

TEST(TestCore, test_array)
{
    using namespace kun;

    // ctor
    {
        Array<u32> a;
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);

        Array<u32> b(20);
        ASSERT_EQ(b.size(), 20);
        ASSERT_GE(b.capacity(), 20);
        ASSERT_NE(b.data(), nullptr);

        Array<u32> c(20, 114514);
        ASSERT_EQ(c.size(), 20);
        ASSERT_GE(c.capacity(), 20);
        ASSERT_NE(c.data(), nullptr);
        for (u32 i = 0; i < 20; ++i) { ASSERT_EQ(c[i], 114514); }

        Array<u32> d(c.data(), c.size() - 5);
        ASSERT_EQ(d.size(), 15);
        ASSERT_GE(d.capacity(), 15);
        ASSERT_NE(d.data(), nullptr);
        for (u32 i = 0; i < 15; ++i) { ASSERT_EQ(d[i], 114514); }

        Array<u32> e({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(e.size(), 6);
        ASSERT_GE(e.capacity(), 6);
        ASSERT_NE(e.data(), nullptr);
        ASSERT_EQ(e[0], 1);
        ASSERT_EQ(e[1], 1);
        ASSERT_EQ(e[2], 4);
        ASSERT_EQ(e[3], 5);
        ASSERT_EQ(e[4], 1);
        ASSERT_EQ(e[5], 4);
    }

    // copy & move
    {
        Array<u32> a(100, 114514);

        Array<u32> b = a;
        ASSERT_EQ(b.size(), a.size());
        ASSERT_GE(b.capacity(), a.size());
        ASSERT_NE(b.data(), nullptr);

        auto       old_size = a.size();
        auto       old_capacity = a.capacity();
        auto       old_data = a.data();
        Array<u32> c = std::move(a);
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);
        ASSERT_EQ(c.size(), old_size);
        ASSERT_EQ(c.capacity(), old_capacity);
        ASSERT_EQ(c.data(), old_data);
    }

    // copy assign & move assign
    {
        Array<u32> a(100, 114514), b, c;

        b = a;
        ASSERT_EQ(b.size(), a.size());
        ASSERT_GE(b.capacity(), a.size());
        ASSERT_NE(b.data(), nullptr);

        auto old_size = a.size();
        auto old_capacity = a.capacity();
        auto old_data = a.data();
        c = std::move(a);
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);
        ASSERT_EQ(c.size(), old_size);
        ASSERT_EQ(c.capacity(), old_capacity);
        ASSERT_EQ(c.data(), old_data);
    }

    // compare
    {
        Array<u32> a({1, 1, 4, 5, 1, 4});
        Array<u32> b(6);
        Array<u32> c(10, 114514);
        Array<u32> d(6, 114514);
        b[0] = 1;
        b[1] = 1;
        b[2] = 4;
        b[3] = 5;
        b[4] = 1;
        b[5] = 4;
        ASSERT_EQ(a, b);
        ASSERT_NE(b, c);
        ASSERT_NE(b, d);
        ASSERT_NE(a, d);
        ASSERT_NE(c, d);
    }

    // [needn't test] getter

    // validate
    {
        Array<u32> a(10), b;

        ASSERT_FALSE(a.isValidIndex(-1));
        ASSERT_FALSE(a.isValidIndex(11));
        ASSERT_TRUE(a.isValidIndex(5));
        ASSERT_TRUE(a.isValidIndex(0));
        ASSERT_TRUE(a.isValidIndex(9));

        ASSERT_FALSE(b.isValidIndex(-1));
        ASSERT_FALSE(b.isValidIndex(0));
        ASSERT_FALSE(b.isValidIndex(1));

        ASSERT_TRUE(a.isValidPointer(a.begin()));
        ASSERT_TRUE(a.isValidPointer(a.begin() + 5));
        ASSERT_TRUE(a.isValidPointer(a.end() - 1));
        ASSERT_FALSE(a.isValidPointer(a.begin() - 1));
        ASSERT_FALSE(a.isValidPointer(a.end()));
    }

    // memory op
    {
        Array<u32> a(50, 114514);

        auto old_capacity = a.capacity();
        auto old_data = a.data();
        a.clear();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), old_capacity);
        ASSERT_EQ(a.data(), old_data);

        a = {1, 1, 4, 5, 1, 4};
        a.release(20);
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 20);

        a.release();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);

        a.reserve(60);
        ASSERT_EQ(a.size(), 0);
        ASSERT_GE(a.capacity(), 60);
        ASSERT_NE(a.data(), nullptr);

        a.shrink();
        ASSERT_EQ(a.size(), 0);
        ASSERT_EQ(a.capacity(), 0);
        ASSERT_EQ(a.data(), nullptr);

        a = Array<u32>(10, 114514);
        a.resize(40, 1145140);
        ASSERT_EQ(a.size(), 40);
        ASSERT_GE(a.capacity(), 40);
        ASSERT_NE(a.data(), nullptr);
        for (u32 i = 0; i < 10; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 10; i < 40; ++i) { ASSERT_EQ(a[i], 1145140); }

        old_capacity = a.capacity();
        a.clear();
        a.resizeUnsafe(36);
        ASSERT_EQ(a.size(), 36);
        ASSERT_EQ(a.capacity(), old_capacity);
        ASSERT_NE(a.data(), nullptr);

        a.clear();
        a.resizeDefault(38);
        ASSERT_EQ(a.size(), 38);
        ASSERT_EQ(a.capacity(), old_capacity);
        ASSERT_NE(a.data(), nullptr);

        a.clear();
        a.resizeZeroed(21);
        ASSERT_EQ(a.size(), 21);
        ASSERT_EQ(a.capacity(), old_capacity);
        ASSERT_NE(a.data(), nullptr);
        for (u32 i = 0; i < 21; ++i) { ASSERT_EQ(a[i], 0); }
    }

    // add
    {
        Array<u32> a(10, 114514);
        a.add(1145140, 5);
        a.add(114514, 20);
        ASSERT_EQ(a.size(), 35);
        ASSERT_GE(a.capacity(), 35);
        ASSERT_NE(a.data(), nullptr);
        for (u32 i = 0; i < 10; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 10; i < 15; ++i) { ASSERT_EQ(a[i], 1145140); }
        for (u32 i = 15; i < 35; ++i) { ASSERT_EQ(a[i], 114514); }

        auto old_capacity = a.capacity();
        a.clear();
        a.addUnique(1);
        a.addUnique(1);
        a.addUnique(4);
        a.addUnique(5);
        a.addUnique(1);
        a.addUnique(4);
        ASSERT_EQ(a.size(), 3);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 4);
        ASSERT_EQ(a[2], 5);

        a.addZeroed();
        ASSERT_EQ(a.size(), 4);
        ASSERT_EQ(a[3], 0);

        a.addUnsafe();
        ASSERT_EQ(a.size(), 5);
        ASSERT_EQ(a[4], 114514);

        a.addDefault();
        ASSERT_EQ(a.size(), 6);

        a.addUnsafe(10);
        ASSERT_EQ(a.size(), 16);

        a.addDefault(10);
        ASSERT_EQ(a.size(), 26);

        ASSERT_EQ(a.capacity(), old_capacity);
    }

    // add at
    {
        Array<u32> a(10, 114514);

        a.addAt(5, 1145140, 20);
        ASSERT_EQ(a.size(), 30);
        for (u32 i = 0; i < 5; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 5; i < 25; ++i) { ASSERT_EQ(a[i], 1145140); }
        for (u32 i = 25; i < 30; ++i) { ASSERT_EQ(a[i], 114514); }

        a.clear();
        a.add(10, 50);
        a.addAtZeroed(25, 100);
        ASSERT_EQ(a.size(), 150);
        for (u32 i = 0; i < 25; ++i) { ASSERT_EQ(a[i], 10); }
        for (u32 i = 25; i < 125; ++i) { ASSERT_EQ(a[i], 0); }
        for (u32 i = 125; i < 150; ++i) { ASSERT_EQ(a[i], 10); }

        a.clear();
        a.add(114514, 30);
        a.addAtUnsafe(15, 10);
        for (u32 i = 0; i < 15; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 25; i < 40; ++i) { ASSERT_EQ(a[i], 114514); }

        a.clear();
        a.add(114514, 30);
        a.addAtDefault(15, 10);
        for (u32 i = 0; i < 15; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 25; i < 40; ++i) { ASSERT_EQ(a[i], 114514); }
    }

    // emplace
    {
        Array<u32> a(10, 114514);
        a.emplace(10);
        ASSERT_EQ(a.size(), 11);
        ASSERT_GE(a.capacity(), 11);
        ASSERT_NE(a.data(), nullptr);
        ASSERT_EQ(a[10], 10);

        a.emplaceAt(5, 25);
        ASSERT_EQ(a.size(), 12);
        ASSERT_GE(a.capacity(), 12);
        ASSERT_NE(a.data(), nullptr);
        for (u32 i = 0; i < 5; ++i) { ASSERT_EQ(a[i], 114514); }
        ASSERT_EQ(a[5], 25);
        for (u32 i = 6; i < 11; ++i) { ASSERT_EQ(a[i], 114514); }
        ASSERT_EQ(a[11], 10);
    }

    // append
    {
        Array<u32> a(20, 114514);
        Array<u32> b(10, 114);

        a.append(b);
        ASSERT_EQ(a.size(), 30);
        ASSERT_GE(a.capacity(), 30);
        for (u32 i = 0; i < 20; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 20; i < 30; ++i) { ASSERT_EQ(a[i], 114); }

        a.append({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(a.size(), 36);
        ASSERT_GE(a.capacity(), 36);
        for (u32 i = 0; i < 20; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 20; i < 30; ++i) { ASSERT_EQ(a[i], 114); }
        ASSERT_EQ(a[30], 1);
        ASSERT_EQ(a[31], 1);
        ASSERT_EQ(a[32], 4);
        ASSERT_EQ(a[33], 5);
        ASSERT_EQ(a[34], 1);
        ASSERT_EQ(a[35], 4);

        a.append(b.data(), 5);
        ASSERT_EQ(a.size(), 41);
        ASSERT_GE(a.capacity(), 41);
        for (u32 i = 0; i < 20; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 20; i < 30; ++i) { ASSERT_EQ(a[i], 114); }
        ASSERT_EQ(a[30], 1);
        ASSERT_EQ(a[31], 1);
        ASSERT_EQ(a[32], 4);
        ASSERT_EQ(a[33], 5);
        ASSERT_EQ(a[34], 1);
        ASSERT_EQ(a[35], 4);
        for (u32 i = 36; i < 41; ++i) { ASSERT_EQ(a[i], 114); }
    }

    // append at
    {
        Array<u32> a(20, 114514);
        Array<u32> b(10, 114);

        a.appendAt(10, b);
        ASSERT_EQ(a.size(), 30);
        ASSERT_GE(a.capacity(), 30);
        for (u32 i = 0; i < 10; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 10; i < 20; ++i) { ASSERT_EQ(a[i], 114); }
        for (u32 i = 20; i < 30; ++i) { ASSERT_EQ(a[i], 114514); }

        a.appendAt(20, {1, 1, 4, 5, 1, 4});
        ASSERT_EQ(a.size(), 36);
        ASSERT_GE(a.capacity(), 36);
        for (u32 i = 0; i < 10; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 10; i < 20; ++i) { ASSERT_EQ(a[i], 114); }
        ASSERT_EQ(a[20], 1);
        ASSERT_EQ(a[21], 1);
        ASSERT_EQ(a[22], 4);
        ASSERT_EQ(a[23], 5);
        ASSERT_EQ(a[24], 1);
        ASSERT_EQ(a[25], 4);
        for (u32 i = 26; i < 36; ++i) { ASSERT_EQ(a[i], 114514); }

        a.appendAt(10, b.data(), 5);
        ASSERT_EQ(a.size(), 41);
        ASSERT_GE(a.capacity(), 41);
        for (u32 i = 0; i < 10; ++i) { ASSERT_EQ(a[i], 114514); }
        for (u32 i = 10; i < 25; ++i) { ASSERT_EQ(a[i], 114); }
        ASSERT_EQ(a[25], 1);
        ASSERT_EQ(a[26], 1);
        ASSERT_EQ(a[27], 4);
        ASSERT_EQ(a[28], 5);
        ASSERT_EQ(a[29], 1);
        ASSERT_EQ(a[30], 4);
        for (u32 i = 31; i < 41; ++i) { ASSERT_EQ(a[i], 114514); }
    }

    // assign
    {
        Array<u32> a(100, 114514);
        Array<u32> b(200, 114);

        a.assign({1, 1, 4, 5, 1, 4});
        ASSERT_EQ(a.size(), 6);
        ASSERT_GE(a.capacity(), 100);
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 1);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 1);
        ASSERT_EQ(a[5], 4);

        a.assign(b.data(), b.size());
        ASSERT_EQ(a.size(), 200);
        ASSERT_GE(a.capacity(), 200);
        for (u32 i = 0; i < 200; ++i) { ASSERT_EQ(a[i], 114); }
    }

    // remove
    {
        Array<u32> a = {1, 1, 4, 5, 1, 4};
        a.add(114514, 20);

        a.removeAt(0, 2);
        ASSERT_EQ(a.size(), 24);
        ASSERT_GE(a.capacity(), 26);
        ASSERT_EQ(a[0], 4);
        ASSERT_EQ(a[1], 5);
        ASSERT_EQ(a[2], 1);
        ASSERT_EQ(a[3], 4);
        for (u32 i = 4; i < 24; ++i) { ASSERT_EQ(a[i], 114514); }

        a.removeAtSwap(0, 2);
        ASSERT_EQ(a.size(), 22);
        ASSERT_EQ(a[0], 114514);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 1);
        ASSERT_EQ(a[3], 4);
        for (u32 i = 4; i < 22; ++i) { ASSERT_EQ(a[i], 114514); }

        a.append({1, 1, 4, 5, 1, 4});
        a.remove(1);
        ASSERT_EQ(a.size(), 27);
        ASSERT_EQ(a[0], 114514);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        for (u32 i = 3; i < 21; ++i) { ASSERT_EQ(a[i], 114514); }
        ASSERT_EQ(a[21], 1);
        ASSERT_EQ(a[22], 1);
        ASSERT_EQ(a[23], 4);
        ASSERT_EQ(a[24], 5);
        ASSERT_EQ(a[25], 1);
        ASSERT_EQ(a[26], 4);

        a.removeSwap(1);
        ASSERT_EQ(a.size(), 26);
        ASSERT_EQ(a[0], 114514);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        for (u32 i = 3; i < 21; ++i) { ASSERT_EQ(a[i], 114514); }
        ASSERT_EQ(a[21], 4);
        ASSERT_EQ(a[22], 1);
        ASSERT_EQ(a[23], 4);
        ASSERT_EQ(a[24], 5);
        ASSERT_EQ(a[25], 1);

        a.removeLast(1);
        ASSERT_EQ(a.size(), 25);
        ASSERT_EQ(a[0], 114514);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        for (u32 i = 3; i < 21; ++i) { ASSERT_EQ(a[i], 114514); }
        ASSERT_EQ(a[21], 4);
        ASSERT_EQ(a[22], 1);
        ASSERT_EQ(a[23], 4);
        ASSERT_EQ(a[24], 5);

        a.removeLastSwap(1);
        ASSERT_EQ(a.size(), 24);
        ASSERT_EQ(a[0], 114514);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 4);
        for (u32 i = 3; i < 21; ++i) { ASSERT_EQ(a[i], 114514); }
        ASSERT_EQ(a[21], 4);
        ASSERT_EQ(a[22], 5);
        ASSERT_EQ(a[23], 4);

        a.removeAllSwap(4);
        ASSERT_EQ(a.size(), 21);
        ASSERT_EQ(a[0], 114514);
        ASSERT_EQ(a[1], 114514);
        ASSERT_EQ(a[2], 5);
        for (u32 i = 3; i < 21; ++i) { ASSERT_EQ(a[i], 114514); }

        a.append({5, 5, 5});
        a.removeAll(5);
        ASSERT_EQ(a.size(), 20);
        for (u32 i = 0; i < 20; ++i) { ASSERT_EQ(a[i], 114514); }
    }

    // [test in remove] remove if

    // [needn't test] modify

    // find
    {
        Array<u32> a({1, 1, 4, 5, 1, 4});

        ASSERT_EQ(a.find(1), a.data() + 0);
        ASSERT_EQ(a.find(4), a.data() + 2);
        ASSERT_EQ(a.find(5), a.data() + 3);
        ASSERT_EQ(a.find(114514), nullptr);
        ASSERT_EQ(a.findLast(1), a.data() + 4);
        ASSERT_EQ(a.findLast(4), a.data() + 5);
        ASSERT_EQ(a.findLast(5), a.data() + 3);
        ASSERT_EQ(a.findLast(114514), nullptr);
    }

    // [test in find] find if

    // [test in find] contain

    // sort
    {
        Array<u32> a(100);
        for (int i = 0; i < 100; ++i) { a[i] = 99 - i; }

        a.sort();
        for (int i = 0; i < 100; ++i) { ASSERT_EQ(a[i], i); }

        a.sortStable(Greater<u32>());
        for (int i = 0; i < 100; ++i) { ASSERT_EQ(a[i], 99 - i); }
    }

    // heap
    {
        Array<u32> a({1, 1, 4, 5, 1, 4, 1});

        ASSERT_FALSE(a.isHeap());

        a.heapify();
        ASSERT_TRUE(a.isHeap());
        ASSERT_EQ(a.heapTop(), 1);

        a.heapPop();
        ASSERT_TRUE(a.isHeap());
        ASSERT_EQ(a.heapTop(), 1);
        ASSERT_EQ(a.size(), 6);

        ASSERT_EQ(a.heapPopGet(), 1);
        ASSERT_TRUE(a.isHeap());
        ASSERT_EQ(a.heapTop(), 1);
        ASSERT_EQ(a.size(), 5);

        a.heapPush(100);
        ASSERT_TRUE(a.isHeap());
        ASSERT_EQ(a.heapTop(), 1);
        ASSERT_EQ(a.size(), 6);

        auto p = a.find(1);
        auto idx = p - a.begin();
        a.heapRemoveAt(idx);
        ASSERT_TRUE(a.isHeap());
        ASSERT_EQ(a.heapTop(), 1);
        ASSERT_EQ(a.size(), 5);

        a.heapSort();
        ASSERT_EQ(a[0], 1);
        ASSERT_EQ(a[1], 4);
        ASSERT_EQ(a[2], 4);
        ASSERT_EQ(a[3], 5);
        ASSERT_EQ(a[4], 100);
    }

    // [test in above code] stack

    // [needn't test] support foreach
}
