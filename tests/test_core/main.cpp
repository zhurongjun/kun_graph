#include <iostream>
#include <kun/core/mimimal.h>
#include <kun/core/archive.h>
#include <kun/core/type_system.h>

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
    kun::TypeFuncTable table;
    kun::makeTypeFuncTable<kun::Object>(table);

    KUN_Assert(false);
    KUN_Assertf(false, "shit {}", 100);

    std::cout << "Object func table: " << std::endl;
    print_func_table_exist(table);

    return 0;
}