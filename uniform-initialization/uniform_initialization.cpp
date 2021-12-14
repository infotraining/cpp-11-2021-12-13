#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace Catch::Matchers;

struct Data
{
    char c;
    double d;
    int data[5];
};

class Point
{
    int x_ {};
    int y_ {};

public:
    Point(int x, int y)
        : x_ {x}
        , y_ {y}
    {
    }

    Point() = default;

    int x() const
    {
        return x_;
    }

    int y() const
    {
        return y_;
    }
};

TEST_CASE("initialization syntax in C++98")
{
    SECTION("simple types")
    {
        int x = 10;
        int y(10);
        REQUIRE(x == y);

        // int z();
        // REQUIRE(z == 0);
    }

    SECTION("aggregates - c-like structures")
    {
        Data d1 {'a', 3.14, {1, 2, 3}};
        Data d2 {'a'};
        Data d3 {};
    }

    SECTION("native arrays")
    {
        int tab1[4] = {1, 2, 3, 4};
        int tab2[4] {1, 2};
        int tab3[4] = {};
    }

    SECTION("classes")
    {
        Point pt1(1, 2);

        REQUIRE(pt1.x() == 1);
        REQUIRE(pt1.y() == 2);
    }

    SECTION("containers")
    {
        std::vector<int> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
    }
}

TEST_CASE("initialization syntax in C++11")
{
    SECTION("simple types")
    {
        int x = 10;
        int y {10};
        REQUIRE(x == y);

        int z {};
        REQUIRE(z == 0);

        int* ptr1 {};
        int* ptr2 = nullptr;
        REQUIRE(ptr1 == ptr2);
    }

    SECTION("aggregates - c-like structures")
    {
        Data d1 {'a', 3.14, {1, 2, 3}};
        Data d2 {'a'};
        Data d3 {};
    }

    SECTION("native arrays")
    {
        int tab1[4] = {1, 2, 3, 4};
        int tab2[4] {1, 2};
        int tab3[4] = {};
    }

    SECTION("classes")
    {
        Point pt1 {1, 2};
        Point pt2 {};
        Point pt3;

        REQUIRE(pt1.x() == 1);
        REQUIRE(pt1.y() == 2);
    }

    SECTION("containers")
    {
        std::vector<int> vec = {1, 2, 3};
    }
}

int64_t get_value()
{
    return 665;
}

TEST_CASE("controlling narrowing conversions")
{
    int64_t value {get_value()};
}

namespace ver_1_0
{
    class Dataset
    {
        int* items_;
        size_t size_;

    public:
        Dataset(std::initializer_list<int> lst)
            : items_ {new int[lst.size()]}
            , size_ {lst.size()}
        {
            std::copy(lst.begin(), lst.end(), items_);
        }
    };
}

namespace ver_2_0
{
    class Dataset
    {
        std::vector<int> items_;

    public:
        Dataset(size_t size, int value) : items_(size, value)
        {}

        Dataset(std::initializer_list<int> lst) : items_{lst}
        {
        }

        std::vector<int>& items() 
        {
            return items_;
        }
    };
}

TEST_CASE("initializer list")
{
    SECTION("native types")
    {
        int x{10};
        auto a{10}; // int - since C++17
    }

    SECTION("auto + initializer_list")
    {
        auto lst1 = {1, 2, 3, 4};
        auto lst2{1, 2, 3}; // int
        auto lst3 = {1}; // std::intializer_list
    }

    ver_2_0::Dataset d1 = {1, 2, 3, 4};

    ver_2_0::Dataset d2{10, 1};
    REQUIRE(d2.items() == std::vector<int>{10, 1});

    ver_2_0::Dataset d3(10, 1);
    REQUIRE(d3.items() == std::vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    std::vector<int> v1(10, 1); // std::vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    std::vector<int> v2{10, 1}; // std::vector<int>{10, 1}

    SECTION("maps + initializer_lists")
    {
        std::map<int, std::string> dict = { {1, "one"}, {2, "two"} };
    }
}