#include "catch.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <tuple>
#include <array>

using namespace std;
using namespace Catch::Matchers;

void print()
{
    std::cout << std::endl;
}

template <typename THead, typename... TTail>
void print(const THead& head, const TTail&... tail)
{
    std::cout << head << " ";
    print(tail...);
}

namespace Cpp17
{
    template <typename THead, typename... TTail>
    void print(const THead& head, const TTail&... tail)
    {
        if constexpr (std::is_arithmetic_v<THead>)
        {
            std::cout << "Number: " << head << " ";
        }
        else
        {
            std::cout << "Other: " << head << " ";
        }

    
        if constexpr (sizeof...(tail) > 0)
        {
            print(tail...);
        }
        else
        {
            std::cout << std::endl;
        }
    }
}

TEST_CASE("variadic templates")
{
    Cpp17::print(1, 3.14, "abc", "def"s);
}


template <size_t... Indices, typename... Ts>
auto select(const std::tuple<Ts...>& tpl)
{
    return std::make_tuple(std::get<Indices>(tpl)...);
}

TEST_CASE("index sequence")
{
    std::tuple<int, double, std::string, std::string> tpl{1, 3.14, "Jan", "Kowalski"};

    REQUIRE(select<0, 3>(tpl) == std::tuple<int, std::string>{1, "Kowalski"});
}

///////////////////////////////////
// constexpr

constexpr int factorial(int n)
{
    return (n == 0) ? 1 : n * factorial(n-1);
}

template <size_t N>
constexpr std::array<size_t, N> create_factorial_lookup()
{
    std::array<size_t, N> results{};

    for(size_t i = 0; i < N; ++i)
        results[i] = factorial(i);

    return results;
}

TEST_CASE("constexpr")
{
    constexpr double pi = 3.14;
    constexpr double pi_2 = pi / 2;

    const int n = 10;
    constexpr int n_2 = n / 2;

    int tab[factorial(5)] = {};

    auto square = [](int x) { return x * x; };
    int tab2[square(10)] = {};

    int x = 10;
    auto x_f = factorial(x);
    std::cout << x_f << "\n";

    constexpr auto lookup_table = create_factorial_lookup<10>();
}

template <typename It, typename Pred>
constexpr auto constexpr_count_if(It first, It last, Pred pred)
{
    size_t count{};
    for(; first != last; ++first)
        if (pred(*first))
            ++count;
    return count;
}

TEST_CASE("constexpr algorithm")
{
    constexpr std::array<int, 10> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    constexpr auto no_of_evens = constexpr_count_if(begin(data), end(data), [](int x) { return x % 2 == 0; });

    std::vector<int> vec = {1, 2, 3, 4};
    const auto no_of_evens_in_vec = constexpr_count_if(begin(vec), end(vec), [](int x) { return x % 2 == 0; });
}