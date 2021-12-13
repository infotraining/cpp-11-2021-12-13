#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

void foo(int* ptr)
{
    if (ptr != nullptr)
    {
        std::cout << "foo(int*: " << *ptr << ")\n";
    }
    else
    {
        std::cout << "foo(int*: nullptr)\n";
    }
}

void foo(nullptr_t)
{
    std::cout << "foo(nullptr_t)\n";
}

void foo(int value)
{
    std::cout << "foo(int: " << value << ")\n";
}

TEST_CASE("NULL is imperfect null pointer")
{
    int* ptr = NULL;

    foo(ptr);
    foo(NULL);
}

TEST_CASE("nullptr is better null pointer")
{
    int* ptr = nullptr;

    foo(ptr);
    foo(nullptr);
}

TEST_CASE("smart pointers + nullptr")
{
    std::unique_ptr<int> uptr1 = nullptr;
    std::unique_ptr<int> uptr2{};

    REQUIRE(uptr1.get() == uptr2.get());

    std::shared_ptr<int> sptr1 = nullptr;
    REQUIRE(sptr1 == nullptr);
}