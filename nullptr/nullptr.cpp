#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void foo(int* ptr)
{
    std::cout << "foo(int*: " << ptr << ")\n";
}

TEST_CASE("NULL is imperfect null pointer")
{
    int* ptr = NULL;

    foo(ptr);
    foo(NULL);
}
