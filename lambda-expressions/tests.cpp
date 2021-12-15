#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include "catch.hpp"

using namespace std;

struct NoCopyAndNoMoveable
{
    int x;
    NoCopyAndNoMoveable(int x)
        : x {x}
    {
    }

    NoCopyAndNoMoveable(const NoCopyAndNoMoveable&) = delete;
    NoCopyAndNoMoveable(NoCopyAndNoMoveable&&) = delete;
};

NoCopyAndNoMoveable create_imposible()
{
    return NoCopyAndNoMoveable {10};
}

TEST_CASE("rvo")
{
    NoCopyAndNoMoveable&& ncm = create_imposible();
}

//////////////////////////////////////////////////
// lambda expressions

class Lambda_756328756238
{
public:
    Lambda_756328756238() = delete;

    auto operator()(int x) const
    {
        return x * x;
    }
};

TEST_CASE("lambda")
{
    auto l1 = [](int32_t x)
    { return x * x; };

    REQUIRE(l1(4) == 16);

    SECTION("is interpreted as")
    {
        // auto l2 = Lambda_756328756238{};
    }
}

class Lambda_6742873649834
{
    const int factor_;

public:
    Lambda_6742873649834(int f)
        : factor_ {f}
    {
    }
    auto operator()(int x) const { return x * factor_; }
};

TEST_CASE("capture")
{
    std::vector<int> large_set;
    int factor = 10;

    SECTION("by value")
    {
        auto multiplier = [=](int x)
        { return x * factor; };
        factor = 20;
        REQUIRE(multiplier(20) == 200);
    }

    SECTION("by ref")
    {
        std::vector<int> vec = {1, 2, 3};
        int sum {};
        std::for_each(begin(vec), end(vec), [&](int x)
            { sum += factor * x; });
    }

    SECTION("by explicit capture")
    {
        std::vector<int> vec = {1, 2, 3};
        int sum {};
        std::for_each(begin(vec), end(vec), [&sum, factor](int x)
            { sum += factor * x; });
    }

    SECTION("by move - generic - since C++14")
    {
        std::unique_ptr<int> uptr = std::make_unique<int>(15);

        auto l = [ptr = std::move(uptr)]()
        { std::cout << *ptr << "\n"; };
    }
}

auto create_generator(int seed)
{
    return [=]() mutable
    { return ++seed; };
}

TEST_CASE("mutable lambda")
{
    auto generator = create_generator(1000);

    REQUIRE(generator() == 1001);
    REQUIRE(generator() == 1002);
}

TEST_CASE("using closure type")
{
    std::vector<int> values = {15, 32, 53, 24, 665, 1};

    auto cmp_by_pointed_values = [](const int* a, const int* b)
    { return *a < *b; };

    std::set<const int*, decltype(cmp_by_pointed_values)> set_pointers(cmp_by_pointed_values);

    for (const auto& item : values)
        set_pointers.insert(&item);

    for (const auto& ptr : set_pointers)
        std::cout << *ptr << " ";
    std::cout << "\n";
}

TEST_CASE("using lambda with stl algorithms")
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::vector<int> squares(vec.size());

    std::transform(begin(vec), end(vec), begin(squares), [](int x)
        { return x * x; });

    REQUIRE(squares == std::vector<int> {1, 4, 9, 16, 25});
}

class Lambda_87365287364886423
{
public:
    template <typename T1, typename T2>
    auto operator()(const T1& a, const T2& b) { return a + b; }
};

void line1() { }

TEST_CASE("generic lambda expressions - since C++14")
{
    auto add = [](const auto& a, const auto& b)
    { 
        line1();
        return a + b; };

    REQUIRE(add(1, 2) == 3);
    REQUIRE(add("one"s, "two"s) == "onetwo"s);
}

namespace LegacyCode
{
    void call_me(void(*f)())
    {
        f();
    }
}

template <typename F>
void call_me(F f)
{
    f();
}

TEST_CASE("storing closure")
{
    SECTION("default - the best option")
    {
        auto add = [](int a, int b)
        { return a + b; };
        REQUIRE(add(1, 2) == 3);
    }

    SECTION("if [] we can store lambda in pointer function")
    {
        int (*subtract)(int, int) = [](int a, int b) { return a - b; };
        REQUIRE(subtract(4, 2) == 2);

        LegacyCode::call_me([]{ std::cout << "calling elvis\n"; });

        auto caller = []() { std::cout << "calling jimmy\n"; };
        LegacyCode::call_me(caller);
    }

    SECTION("std::function")
    {
        std::function<void()> callback;

        int x = 42;
        callback = [x] { std::cout << x << "\n"; };
        callback();
    }
}