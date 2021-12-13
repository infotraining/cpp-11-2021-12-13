#include "catch.hpp"
#include "gadget.hpp"

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

void have_fun(const Gadget& cg)
{
    puts(__PRETTY_FUNCTION__);
    cg.use();
}

void have_fun(Gadget&& g)
{
    puts(__PRETTY_FUNCTION__);
    g.use();
}

void use(Gadget& g)
{
    have_fun(g);
}

void use(const Gadget& g)
{
    have_fun(g);
}

void use(Gadget&& g)
{
    have_fun(std::move(g));
}

// TEST_CASE("4---")
// {
//     std::cout << "\n--------------------------\n\n";
// }

// TEST_CASE("custom forwarding")
// {
//     Gadget g{1, "gadget"};
//     const Gadget cg{2, "const-gadget"};

//     use(g);
//     use(cg);
//     use(Gadget{3, "temp-gadget"});
// }