#include <math/analytic_roots.h>

#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;

TEST_CASE("Constant analytical root")
{
    CHECK_THROWS_AS(linearRoot(0., 0.), std::runtime_error);
    CHECK_THROWS_AS(firstRealQuadraticRoot(0., 0., 0.), std::runtime_error);
    CHECK_THROWS_AS(firstRealCubicRoot(0., 0., 0., 0.), std::runtime_error);
}

TEST_CASE("Linear analytical root")
{
    CHECK(linearRoot(1., 2.) == -2.);
    CHECK(firstRealQuadraticRoot(0., 1., 2.) == -2.);
    CHECK(firstRealCubicRoot(0., 0., 1., 2.) == -2.);
}

TEST_CASE("Quadratic analytical root")
{
    CHECK(firstRealQuadraticRoot(1., -3., 2.) == 2.);
    CHECK_THROWS_AS(firstRealQuadraticRoot(1., 0., 1.), std::runtime_error);
    CHECK(firstRealCubicRoot(0., 1., -3., 2.) == 2.);
    CHECK_THROWS_AS(firstRealCubicRoot(0., 1., 0., 1.), std::runtime_error);
}

TEST_CASE("Cubic analytical root")
{
    CHECK(firstRealCubicRoot(1., -6., 11., -6.) == 1.);       // Triple roots 1, 2 and 3
    CHECK(firstRealCubicRoot(27., -162., 324., -216.) == 2.); // Triple root 2, but as r,q=0 -> m=0
    CHECK(firstRealCubicRoot(1., 1., 1., 1.) == -1.);         // Standard case
    CHECK(firstRealCubicRoot(2., 1., 2., 1.) == -0.5);        // Standard case
}
