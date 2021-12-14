#include "catch.hpp"
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace Catch::Matchers;

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& lref_name = name;

        const std::string& lref_full_name = full_name(name, "kowalski");
        // lref_full_name[0] = 'p';
    }

    SECTION("C++11")
    {
        std::string&& rref_full_name = full_name(name, "kowalski"); // rv-ref wydłuża czas życia obiektu tymczasowego i umożliwia nazwany dostęp
        rref_full_name[0] = 'p';
        std::cout << rref_full_name << "\n";

        // std::string&& rref_name = name; // ERROR - cannot bind lvalue to rvalue-ref
    }
}

namespace Explain
{
    template <typename T>
    class Vector
    {
        std::vector<T> items_;

    public:
        void push_back(const T& value)
        {
            std::cout << "push_back copies " << value << " into container\n";
        }

        void push_back(T&& value)
        {
            std::cout << "push_back moves " << value << " into container\n";
        }
    };
}

TEST_CASE("vector + push_back()")
{
    Explain::Vector<std::string> vec;

    {
        std::string name = "jan";
        vec.push_back(name); // void push_back(const T& value)
        vec.push_back(std::move(name)); // void push_back(T&& value) 
        vec.push_back("kowalski"); // void push_back(T&& value)

        name = "adam"; // OK    
    }
}

TEST_CASE("std::move")
{
    SECTION("primitive types")
    {
        int x = 42;
        int target = std::move(x); // x is copied do target
        REQUIRE(x == target);

        int* ptr = &x;
        int* ptr_target = std::move(ptr); // ptr is copied to target
        REQUIRE(ptr == ptr_target);
    }

    SECTION("classes")
    {
        std::vector<int> vec = {1, 2, 3};
        std::vector<int> backup = vec; // copy constructor
        std::vector<int> target = std::move(vec); // move constructor

        REQUIRE(vec.size() == 0);
        REQUIRE(target == std::vector<int>{1, 2, 3});
    }
}