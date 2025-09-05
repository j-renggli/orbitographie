#include <math/colour/colour.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;

namespace
{

void check(const Colour& col, float r, float g, float b, float a = 1.f)
{
    INFO(col.r() << "f, " << col.g() << "f, " << col.b() << "f");
    CHECK(col.r() == Catch::Approx(r));
    CHECK(col.g() == Catch::Approx(g));
    CHECK(col.b() == Catch::Approx(b));
    CHECK(col.a() == Catch::Approx(a));
}

} // namespace

TEST_CASE("Colour basic constructor and limits")
{
    check(Colour(0., 0., 0.), 0., 0., 0.);
    check(Colour(1., 1., 1.), 1., 1., 1.);
    check(Colour(0., 0.5, 1.), 0., 0.5, 1.);
    CHECK_THROWS_AS(Colour(-0.1, 0., 0.), std::runtime_error);
    CHECK_THROWS_AS(Colour(1.1, 0., 0.), std::runtime_error);
    CHECK_THROWS_AS(Colour(0., -0.1, 0.), std::runtime_error);
    CHECK_THROWS_AS(Colour(0., 1.1, 0.), std::runtime_error);
    CHECK_THROWS_AS(Colour(0., 0., -0.1), std::runtime_error);
    CHECK_THROWS_AS(Colour(0., 0., 1.1), std::runtime_error);
}

TEST_CASE("Colour as hex")
{
    CHECK(Colour(0., 0., 0., 0.).hex() == "0x00000000");
    CHECK(Colour(0., 0.5, 0.).hex() == "0x007f00ff");
    CHECK(Colour(0.2, 0.4, 0.6, 0.8).hex() == "0x336699cc");
    CHECK(Colour(1., 1., 1., 1.).hex() == "0xffffffff");
}

TEST_CASE("Colour XYZ to sRGB")
{
    check(Colour::fromXYZ(0., 0., 0.), 0., 0., 0.);
    check(Colour::fromXYZ(1., 1., 1.), 1.f, 0.900258f, 0.883567f);
}

TEST_CASE("Blackbody colour")
{
    check(blackBodyColour(1000.f), 1.f, 0.320656f, 0.f);
    check(blackBodyColour(1500.f), 1.f, 0.538373f, 0.f);
    check(blackBodyColour(2000.f), 1.f, 0.544072f, 0.0876681f);
    check(blackBodyColour(2500.f), 1.f, 0.64249f, 0.28544f);
    check(blackBodyColour(3000.f), 1.f, 0.717548f, 0.423672f);
    check(blackBodyColour(3500.f), 1.f, 0.777209f, 0.540421f);
    check(blackBodyColour(4000.f), 1.f, 0.825813f, 0.64196f);
    check(blackBodyColour(4500.f), 1.f, 0.866066f, 0.73113f);
    check(blackBodyColour(5000.f), 1.f, 0.899809f, 0.809815f);
    check(blackBodyColour(5500.f), 1.f, 0.928364f, 0.879476f);
    check(blackBodyColour(6000.f), 1.f, 0.952726f, 0.94132f);
    check(blackBodyColour(6500.f), 1.f, 0.973659f, 0.996378f);
    check(blackBodyColour(7000.f), 0.956449f, 0.948575f, 1.f);
    check(blackBodyColour(7500.f), 0.917811f, 0.924719f, 1.f);
    check(blackBodyColour(8000.f), 0.885676f, 0.904562f, 1.f);
    check(blackBodyColour(8500.f), 0.858593f, 0.88733f, 1.f);
    check(blackBodyColour(9000.f), 0.835505f, 0.872449f, 1.f);
    check(blackBodyColour(9500.f), 0.815624f, 0.859485f, 1.f);
    check(blackBodyColour(10000.f), 0.798351f, 0.848102f, 1.f);
}
