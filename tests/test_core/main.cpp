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

    // test sort
    kun::u32 val_arr[1000];
    for (kun::u32 i = 0; i < 1000; ++i) { val_arr[i] = 999 - i; }
    kun::algo::mergeSort(val_arr, val_arr + 1000);
    KUN_Verify(kun::algo::isSorted(val_arr, val_arr + 1000));

    return 0;
}