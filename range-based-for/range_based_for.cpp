#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

TEST_CASE("range-based-for")
{
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    SECTION("iterating over container in C++98")
    {
        for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
        {
            (*it) *= 2;
        }

        for (std::vector<int>::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }

    SECTION("C++11")
    {
        for (int& item : vec)
            item *= 2;

        SECTION("is interpreted as")
        {
            auto&& container = vec;
            auto it = container.begin();
            auto end = container.end();
            for (; it != end; ++it)
            {
                int& item = *it;
                item *= 2;
            }
        }

        for (const int& item : vec)
            std::cout << item << " ";
        std::cout << "\n";

        for (const auto& item : vec)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    SECTION("arrays")
    {
        int tab[] = {1, 2, 3};

        for (const auto& item : tab)
            std::cout << item << " ";
        std::cout << "\n";
    }

    SECTION("arrays")
    {
        int tab[] = {1, 2, 3};

        for (const auto& item : tab)
            std::cout << item << " ";
        std::cout << "\n";

        SECTION("is interpreted as")
        {
            auto&& container = tab;
            auto it = std::begin(tab);
            auto end = std::end(tab);
            for (; it != end; ++it)
            {
                const auto& item = *it;
                std::cout << item << " ";
            }
            std::cout << "\n";
        }
    }

    SECTION("initializer lists")
    {
        auto il = {1, 2, 3};

        for (const auto& item : il)
            std::cout << item << " ";
        std::cout << "\n";

        for(const std::string& word : { "one"s, "two"s, "three"s })
            std::cout << word << "\n";
    }
}

/////////////////////////////////////////////////
// dangling references in range-based-for loop!!!

class User
{
    std::string name_;

public:
    User(const std::string& name) : name_(name)
    {
    }

    const std::string& name() const
    
    {
        return name_;
    }
};

TEST_CASE("beware of dangling references in range-based-loop")
{    
    for(const auto& c : User{"Jan Kowalski"}.name())
    {
        std::cout << c << " ";
    }
    std::cout << "\n";

    SECTION("is interpreted as")
    {
        auto&& container = User{"Jan Kowalski"}.name(); // dangling reference to destroyed temp object
        auto first = begin(container);
        auto last = end(container);
        for(; first != last; ++first)
        {
            const auto& c = *first;
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
}