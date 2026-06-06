#include "../src/keplersolver/hyperbolic.h"

#include "utils/kepler_checks.h"

#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace orbit;
using namespace coordinates;

namespace
{

const GravitationalParam mu{3.986004418e14};
constexpr double t0{0.};
constexpr qty::Second time0{t0};
constexpr qty::Second time1{t0 + 3600.};
const CenterOfMass com(mu, time0, Cartesian{{{1e9, 0., 0.}}, {{0., 1e6, 0.}}});

} // namespace

TEST_CASE("Hyperbolic construction")
{
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Hyperbolic);

    auto solver = UniversalKeplerSolver::create(CenterOfMass{com});
    REQUIRE(dynamic_cast<HyperbolicKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time1);
    const double guess = solver->initialGuess();
    CHECK(guess == Catch::Approx(2.08637e-06));
    CHECK(solver->f(guess) == Catch::Approx(365.7294519162));
    CHECK(solver->df(guess) == Catch::Approx(-2089864116.1965370178));

    CHECK(1e6 * root == Catch::Approx(1.990746694)); // 1.9928
    CHECK(solver->f(root) == Catch::Approx(-7.7968647295));
    CHECK(solver->df(root) == Catch::Approx(-1728794068.021689415));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Catch::Approx(0.9999993621));
    CHECK(factors.g == Catch::Approx(3965.727039547));
    CHECK(1e6 * factors.df == Catch::Approx(0.0007563844));
    CHECK(factors.dg == Catch::Approx(0.9999998724));
    CHECK(factors.f * factors.dg - factors.df * factors.g == Catch::Approx(1.));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Catch::Approx(0.999999));
    CHECK(rootFactors.g == Catch::Approx(3592.2009099034));
    CHECK(1e9 * rootFactors.df == Catch::Approx(0.828238));
    CHECK(rootFactors.dg == Catch::Approx(1.));
    CHECK(rootFactors.f * rootFactors.dg - rootFactors.df * rootFactors.g == Catch::Approx(1.));
}

TEST_CASE("Hyperbolic orbit")
{
    // Prepare solutions at a few selected times
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Hyperbolic);

    const std::vector<double> t = {
        -300.,
        -250.,
        -200.,
        -150.,
        -100.,
        -50.,
        0.,
        50.,
        100.,
        150.,
        200.,
        250.,
    };

    test::Results expected;
    expected.guess = {
        -2.95691e-07,
        -2.47474e-07,
        -1.98693e-07,
        -1.49444e-07,
        -9.98342e-08,
        -4.99792e-08,
        0.,
        4.99792e-8,
        9.98342e-8,
        1.49444e-7,
        1.98693e-7,
        2.47474e-7,

    };
    expected.s = {
        -2.95673e-07,
        -2.47466e-07,
        -1.9869e-07,
        -1.49443e-07,
        -9.98341e-08,
        -4.99792e-08,
        0.,
        4.99792e-8,
        9.98341e-8,
        1.49443e-7,
        1.9869e-7,
        2.47466e-7,
    };
    expected.x = {
        -17.5506,
        -12.2675,
        -7.89384,
        -4.45931,
        -1.98804,
        -0.49794,
        0,
        -0.49794,
        -1.98804,
        -4.45931,
        -7.89384,
        -12.2675,
    };
    expected.y = {
        -3e+08,
        -2.5e+08,
        -2e+08,
        -1.5e+08,
        -1e+08,
        -5e+07,
        0,
        5e7,
        1e8,
        1.5e8,
        2e8,
        2.5e8,
    };
    expected.vx = {
        0.125087721,
        0.1028142793,
        0.0813306869,
        0.0604664813,
        0.0400598128,
        0.0199549343,
        0.0,
        -0.0199549343,
        -0.0400598128,
        -0.0604664813,
        -0.0813306869,
        -0.1028142793,
    };
    expected.vy = {
        -0.0183589656,
        -0.0126570065,
        -0.0080533188,
        -0.0045097562,
        -0.0019980065,
        -0.0004985615,
        0.0,
        -0.0004985615,
        -0.0019980065,
        -0.0045097562,
        -0.0080533188,
        -0.0126570065,
    };

    auto solver = UniversalKeplerSolver::create(CenterOfMass{com});

    const auto c0 = com.initialCoordinates();
    const double h0 = c0.position().cross(c0.velocity())[2].value();

    test::Results().computeAndCheck(*solver, t, expected, h0, com.initialCoordinates());
}
