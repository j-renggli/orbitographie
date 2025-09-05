#include <math/solver/bisection.h>

#include "utils/functions.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;
using namespace solver;
using namespace test;

TEST_CASE("Bisection with y = x")
{
    Linear lin;

    CHECK_THROWS_AS(Bisection::findRoot(lin, Range<double>(1., 2.)), std::runtime_error);
    CHECK_THROWS_AS(Bisection::findRoot(lin, Range<double>(-2., -1.)), std::runtime_error);

    CHECK(Bisection::findRoot(lin, Range<double>(0., 1.5)) == 0.);
    CHECK(static_cast<const ISolver&>(Bisection()).findRoot(lin, Range<double>(0., 1.5)) == 0.);
}

TEST_CASE("Bisection with y = x*x -3x -6")
{
    Quadratic lin;

    SECTION("left root") { CHECK(Bisection::findRoot(lin, Range<double>(-2., 3.)) == Catch::Approx(-1.3722813204)); }

    SECTION("right root") { CHECK(Bisection::findRoot(lin, Range<double>(0., 5.)) == Catch::Approx(4.3722813204)); }
}
