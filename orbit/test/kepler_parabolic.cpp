#include "../src/keplersolver/parabolic.h"

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
const CenterOfMass com(mu, time0, Cartesian{{{2. * mu.value() / 1e6, 0., 0.}}, {{0., 1e3, 0.}}}, nullptr);
} // namespace

TEST_CASE("Parabolic construction")
{
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Parabolic);

    auto solver = UniversalKeplerSolver::create(com);
    REQUIRE(dynamic_cast<ParabolicKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time1);
    const double guess = solver->initialGuess();
    CHECK(guess == Catch::Approx(4.5158e-06));
    CHECK(1e9 * solver->f(guess) == Catch::Approx(1.3501449));
    CHECK(solver->df(guess) == Catch::Approx(797204947.8064689636));

    CHECK(root == Catch::Approx(4.51579e-06));
    CHECK(1e12 * solver->f(root) == Catch::Approx(0.45474735089));
    CHECK(solver->df(root) == Catch::Approx(797204947.8064689636));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Catch::Approx(0.9999949248));
    CHECK(factors.g == Catch::Approx(3599.9939234559));
    CHECK(1e6 * factors.df == Catch::Approx(-0.0028322658));
    CHECK(factors.dg == Catch::Approx(0.9999949476));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Catch::Approx(0.999995));
    CHECK(rootFactors.g == Catch::Approx(3599.99));
    CHECK(rootFactors.df == Catch::Approx(-2.83227e-09));
    CHECK(rootFactors.dg == Catch::Approx(0.999995));
}

TEST_CASE("Parabolic orbit")
{
    // Prepare solutions at a few selected times
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Parabolic);

    const std::vector<double> t = {
        -2400.,
        -2000.,
        -1600.,
        -1200.,
        -800.,
        -400.,
        0.,
        400.,
        800.,
        1200.,
        1600.,
        2000.,
    };

    test::Results expected;
    expected.guess = {
        -3.01053e-06,
        -2.50878e-06,
        -2.00702e-06,
        -1.50527e-06,
        -1.00351e-06,
        -5.01756e-07,
        0,
        5.01756e-07,
        1.00351e-06,
        1.50527e-06,
        2.00702e-06,
        2.50878e-06,

    };
    expected.s = expected.guess;
    expected.x = {
        -1806.32,
        -1254.39,
        -802.808,
        -451.58,
        -200.702,
        -50.1756,
        0,
        -50.1756,
        -200.702,
        -451.58,
        -802.808,
        -1254.39,
    };
    expected.y = {
        -2.4e+06,
        -2e+06,
        -1.6e+06,
        -1.2e+06,
        -800000,
        -400000,
        0,
        400000,
        800000,
        1.2e+06,
        1.6e+06,
        2e+06,
    };
    expected.vx = {
        1.50526,
        1.25439,
        1.00351,
        0.752633,
        0.501755,
        0.250878,
        -0,
        -0.250878,
        -0.501755,
        -0.752633,
        -1.00351,
        -1.25439,
    };
    expected.vy = {
        -0.00226582,
        -0.00157349,
        -0.00100703,
        -0.000566456,
        -0.000251759,
        -6.29397e-05,
        -0,
        -6.29397e-05,
        -0.000251759,
        -0.000566456,
        -0.00100703,
        -0.00157349,
    };

    auto solver = UniversalKeplerSolver::create(com);

    const auto c0 = com.initialCoordinates();
    const double h0 = c0.position().cross(c0.velocity())[2].value();

    test::Results().computeAndCheck(*solver, t, expected, h0, com.initialCoordinates());
}
