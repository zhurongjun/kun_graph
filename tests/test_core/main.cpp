#include <iostream>
#include <kun/core/mimimal.h>
#include <kun/core/archive.h>
#include <kun/core/type_system.h>
#include <kun/core/math.h>
#include <kun/core/algo.hpp>
#include <glm/vec2.hpp>

void print_func_table_exist(const kun::TypeFuncTable& table)
{
#define KUN_PRINT_EXIST(member) #member << " exist: " << (table.member ? "true" : "false") << std::endl

    std::cout << KUN_PRINT_EXIST(ctor) << KUN_PRINT_EXIST(init) << KUN_PRINT_EXIST(shutdown) << KUN_PRINT_EXIST(dtor)

              << KUN_PRINT_EXIST(serialize)

              << KUN_PRINT_EXIST(copy) << KUN_PRINT_EXIST(move) << KUN_PRINT_EXIST(assign) << KUN_PRINT_EXIST(move_assign)

              << KUN_PRINT_EXIST(swap) << KUN_PRINT_EXIST(hash)

              << KUN_PRINT_EXIST(add) << KUN_PRINT_EXIST(sub) << KUN_PRINT_EXIST(mul) << KUN_PRINT_EXIST(div)

              << KUN_PRINT_EXIST(equal) << KUN_PRINT_EXIST(less) << KUN_PRINT_EXIST(greater);

#undef KUN_PRINT_EXIST
}

int main()
{
    // test func table
    kun::TypeFuncTable table;
    kun::makeTypeFuncTable<int>(table);
    std::cout << "Object func table: " << std::endl;
    print_func_table_exist(table);

    // test memswap
    kun::i32 vals[1000];
    for (int i = 0; i < 1000; ++i) { vals[i] = i; }
    kun::memory::memswap(vals, vals + 500, sizeof(kun::i32) * 500);
    for (int i = 0; i < 500; ++i)
    {
        KUN_Verify(vals[i] == 500 + i);
        KUN_Verify(vals[500 + i] == i);
    }
    for (int i = 0; i < 1000; ++i) { vals[i] = i * 2 + 1; }

    auto lower = kun::algo::lowerBound(vals, vals + 1000, 1001);
    auto upper = kun::algo::upperBound(vals, vals + 1000, 1001);
    auto found = kun::algo::binarySearch(vals, vals + 1000, 1001);

    std::cout << "lower: " << *lower << std::endl << "upper: " << *upper << std::endl << "found: " << (found != nullptr) << std::endl;

    return 0;
}