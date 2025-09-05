#include <math/solver/newton_raphson.h>

#include "utils/functions.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;
using namespace solver;
using namespace test;

TEST_CASE("Newton Raphson with y = x")
{
    Linear lin;

    CHECK(NewtonRaphson::findRoot(lin, 1.5) == 0.);
    CHECK(NewtonRaphson::findRoot(lin, -1.5) == 0.);

    CHECK(NewtonRaphson::findRoot(lin, 0.75) == 0.);
    CHECK(static_cast<const ISolver&>(NewtonRaphson()).findRoot(lin, Range<double>(0., 1.5)) == 0.);
}

TEST_CASE("Newton Raphson with y = x*x -3x -6")
{
    Quadratic lin;

    SECTION("left root") { CHECK(NewtonRaphson::findRoot(lin, 0.5) == Catch::Approx(-1.3722813204)); }

    SECTION("right root") { CHECK(NewtonRaphson::findRoot(lin, 2.5) == Catch::Approx(4.3722813204)); }
}
