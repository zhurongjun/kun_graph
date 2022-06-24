#include <iostream>

struct Test
{
    Test()
        : a(1)
        , b(2)
    {
        a = 2;
        b = 1;
    }

    int a;
    int b;
};

int main()
{
    std::cout << "shit" << std::endl;
    return 0;
}