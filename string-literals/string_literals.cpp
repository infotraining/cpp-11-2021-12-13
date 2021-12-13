#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std::literals;

TEST_CASE("raw-string literals")
{
    std::string str1 = "c:\nasz katalog\backup\n";
    std::cout << str1;

    std::string str2 = R"(c:\nasz katalog\backup)";
    std::cout << str2 << std::endl;

    std::string lines = R"(line1
line2
line3
)";

    std::cout << lines;

    std::string text = R"kstring(value: "(13)" )kstring";
    std::cout << text << "\n";
}

TEST_CASE("std::string literal - since C++14")
{
    auto ctext = "text"; // const char*
    auto text = "text"s; // std::string
}

TEST_CASE("std::chrono")
{
    auto timespan = 100ms; // std::chrono::milliseconds

    auto timespan_2 = 1s; // std::chrono::seconds
}

enum class Currency
{
    usd, eur, pln
};

struct Money
{
    long double value;
    Currency currency;
};

Money operator-(const Money& m)
{
    return Money{-m.value, m.currency};
}

Money operator""_KUSD(long double value)
{
    return Money{value, Currency::usd};
}

Money operator""_EUR(unsigned long long value)
{
    return Money{static_cast<double>(value), Currency::eur};
}

TEST_CASE("custom literals")
{
    auto price_1 = 100.99_KUSD;

    auto price_2 = -200_EUR;
}