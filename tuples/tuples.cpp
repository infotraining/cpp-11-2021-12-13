#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

TEST_CASE("tuples")
{
    std::vector<int> data = { 6, 2, 5, 42, 665, 234, 65, 44, 1 };

    int min, max;
    double avg;

    // write a function that finds minimum, maximum & average value in data

    REQUIRE(min == 1);
    REQUIRE(max == 665);
    REQUIRE(avg == Approx(118.222));
}