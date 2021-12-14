#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

class IGadget
{
public:
    virtual void use() const = 0;
    virtual ~IGadget() = default;
};

struct Noncopyable
{
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
protected:
    Noncopyable() = default;
};

class Gadget : public IGadget, public Noncopyable
{
    int id_ {-1};
    std::string name_ = "unknown";

public:
    Gadget() = default;

    Gadget(int id) : Gadget{id, "Gadget-"s + std::to_string(id)} // delegating contruction to another constructor
    {
    }

    Gadget(int id, std::string name)
        : id_ {id}
        , name_ {std::move(name)}
    {
    }

    int id() const
    {
        return id_;
    }

    std::string name() const
    {
        return name_;
    }

    void use() const override
    {
        std::cout << "Using gadget: " << id() << " - " << name() << "\n";
    }
};

class SuperGadget : public Gadget
{
public:
    using Gadget::Gadget; // inheritance of constructors

    SuperGadget(int id) : SuperGadget{id, "SuperGadget-"s + std::to_string(id)} // delegating contruction to another constructor
    {
    }

    void use() const override
    {
        std::cout << "Using super gadget: " << id() << " - " << name() << "\n";
    }
};

class HyperGadget final : public SuperGadget
{
    int extra; // potential problem - uninitialized field
public:
    using SuperGadget::SuperGadget;

    void use() const override
    {
        std::cout << "Using hyper gadget: " << id() << " - " << name() << "\n";
    }
};

TEST_CASE("inheritance of constructors")
{
    SuperGadget sg1 {1, "super-gadget"};
    SuperGadget sg2 {4};
    sg2.use();
    SuperGadget sg3;

    Gadget& g_ref = sg1;
    g_ref.use();
}

void calculate(int n)
{
    std::cout << "n: " << n << "\n";
}

void calculate(double) = delete;

TEST_CASE("delete common function")
{
    calculate(42);
    //calculate(3.14);
}