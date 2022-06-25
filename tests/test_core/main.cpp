#include <iostream>
#include <kun/core/mimimal.h>
#include <kun/core/archive.h>
#include <kun/core/type_system.h>

int main()
{
    kun::Name a("shit");
    kun::Name b("fuck");
    kun::Name c("ohhhh");
    kun::Name d("shit");
    kun::Name e("fuck");

    kun::String a_str(a.str());
    kun::String b_str(b.str());
    kun::String c_str(c.str());
    kun::String d_str(d.str());
    kun::String e_str(e.str());

    std::cout << "a: " << a_str.data() << std::endl
              << "b: " << b_str.data() << std::endl
              << "c: " << c_str.data() << std::endl
              << "d: " << d_str.data() << std::endl
              << "e: " << e_str.data() << std::endl;

    return 0;
}