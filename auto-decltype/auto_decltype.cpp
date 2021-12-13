#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;
using namespace Catch::Matchers;

int foo(int arg)
{
    return arg;
}

#if __cplusplus > 201703L

void deduce(auto arg)
{
    puts(__PRETTY_FUNCTION__);
}

void deduce_with_ref(auto& arg)
{
    puts(__PRETTY_FUNCTION__);
}

#else

template <typename T>
void deduce(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce_with_ref(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

#endif

TEST_CASE("simple declarations")
{
    auto number = 42; // int
    const auto pi = 3.14; // double
    auto e = 2.71f; // float

    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();

    const auto& first_item = *it;
    auto* ptr_first_item = &first_item;

    foo(first_item);
    // foo(pi);
}

TEST_CASE("auto type deduction - case 1")
{
    int x = 10;
    auto ax1 = x; // int
    deduce(x);

    const int cx = 42;
    auto ax2 = cx; // int
    deduce(cx);

    int& ref_x = x;
    auto ax3 = ref_x; // int
    deduce(ref_x);

    const int& cref_x = x;
    auto ax4 = cref_x; // int
    deduce(cref_x);

    int tab[10];
    auto ax5 = tab; // int*
    deduce(tab);

    auto ax6 = foo; // int(*)(int)
    auto* ax7 = foo; // int(*)(int)
    deduce(foo);
}

TEST_CASE("auto type deduction - case 2")
{
    int x = 10;
    auto& a1 = x; // int&
    deduce_with_ref(x);

    const int cx = 42;
    auto& a2 = cx; // const int&
    deduce_with_ref(cx);

    int& ref_x = x;
    auto& a3 = ref_x; // int&

    const int& cref_x = x;
    auto& a4 = cref_x; // const int&

    int tab[10];
    auto& a5 = tab; // int(&)[10]

    auto& a6 = foo; // int(&)(int)
}

TEST_CASE("using auto for iteration")
{
    std::cout << "\n--------------------\n";
    int data[] = {1, 2, 3, 4};

    for(auto it = std::begin(data); it != std::end(data); ++it)
    {
        *it *= 2;
    }

    for(auto it = std::cbegin(data); it != std::cend(data); ++it)
    {
        std::cout << *it << " ";
    }   
    std::cout << "\n";
}

TEST_CASE("declaration syntax - auto")
{
    SECTION("without")
    {
        int x1 = 10;
        int x2 = int(10);
        int x3(10);
        int x4{10};
    }

    SECTION("with auto")
    {
        auto a1 = 10;  // int
        auto a2 = int(10);  // int
        auto a3(10); // int
        //auto a4{10}; // std::initializer_list<int> - in C++11/14
        auto a4{10}; // int - since C++17

        auto list1 = {1, 2, 3, 4}; // std::initializer_list<int>
    }
}

struct NoDefault
{
private:
    NoDefault() {}
public:
    int foo() {
        return 42;
    }
};

TEST_CASE("decltype")
{
    int16_t x = 10;
    decltype(x) y = 20;

    std::vector<decltype(x)> vec = { x, y };

    auto backup = vec; // copy
    decltype(vec) empty; // no-copy

    auto& ref_last_item = vec.back();

    std::map<int, std::string> dict = { {1, "one"}, {2, "two"} };

    decltype(dict[3]) ref_value = dict[2];

    REQUIRE(dict.size() == 2);

    std::vector<decltype(std::declval<NoDefault>().foo())> container;
}

template <typename T>
auto multiply(const T& a, const T& b) -> decltype(a * b)
{
    return a * b;
}

TEST_CASE("multiply")
{
    int x = 10;
    int y = 20;

    int result = multiply(x, y);
}