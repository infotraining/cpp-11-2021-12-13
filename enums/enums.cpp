#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <map>

using namespace std;
using namespace Catch::Matchers;

enum DayOfWeek : uint16_t; // forward declaration

void set_schedule(DayOfWeek);

enum DayOfWeek : uint16_t { mon = 1, tue, wed, thd, fri, sat, sun = 256}; // definition

TEST_CASE("enum types in C++98")
{
    DayOfWeek today = mon;
    REQUIRE(today == 1);

    // today = sun;
    // std::underlying_type_t<DayOfWeek> value = today;
    // ++value;
    // REQUIRE(value == 2);

    // DayOfWeek tomorrow = static_cast<DayOfWeek>(value);
    // REQUIRE(tomorrow == tue);
}

enum class Engine : uint8_t { petrol = 1, diesel, tdi = 2, wankel, hybrid, electric, hydrogen_cell };

TEST_CASE("enum class")
{
    REQUIRE(Engine::diesel == Engine::tdi);

    auto index = static_cast<underlying_type_t<Engine>>(Engine::hybrid);

    index = 5;

    Engine e = static_cast<Engine>(index);

    REQUIRE(e == Engine::electric);
}

TEST_CASE("enum classes in C++17")
{
    // Engine e1 = 5;
    // Engine e2(5);
    Engine e3{5};
}

TEST_CASE("bytes in C++")
{
    int price = 0b1111'1111;

    std::byte b1{0b0000'1111};
    std::byte b2{0b1111'0000};

    std::bitset<8> bs{std::to_integer<uint8_t>(b2)};
    std::cout << "bs: " << bs << "\n";

    REQUIRE((b1 | b2) == std::byte{0b1111'1111});

    //b1 = b1 + b2;

    std::cout << std::to_integer<int32_t>(b2) << "\n";
}

/////////////////////////////////////////////////////////////////////////
// using enum class as index in vector

template <typename EnumClass>
struct UseEnumAsIndex : std::false_type
{};

template <typename EnumClass, typename IndexType = size_t, typename = std::enable_if_t<UseEnumAsIndex<EnumClass>::value>>
constexpr IndexType as_index(EnumClass enum_value)
{
    static_assert(sizeof(IndexType) >= sizeof(std::underlying_type_t<EnumClass>), 
        "Size of IndexType must be at least equal to size of underlying type in enum");

    return static_cast<IndexType>(enum_value);
}

enum class Country { pl, us, fr, gb, it};

// turn-on as_index for Country enum
template <>
struct UseEnumAsIndex<Country> : std::true_type
{};

TEST_CASE("enum class as index")
{
    std::map<Country, std::string> dict = { {Country::pl, "Poland"}, {Country::us, "United States"} }; 

    std::cout << dict[Country::pl] << "\n";

    std::vector<std::string> country_names = { "Poland", "United States" }; 

    std::cout << country_names[as_index(Country::pl)] << "\n";
}