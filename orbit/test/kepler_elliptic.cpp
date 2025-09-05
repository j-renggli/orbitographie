#include "../src/keplersolver/elliptic.h"

#include "utils/kepler_checks.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

using namespace galaxias;
using namespace orbit;
using namespace coordinates;

namespace
{
const GravitationalParam mu{3.986004418e14};
constexpr double t0{0.};
constexpr qty::Second time0{t0};
constexpr qty::Second time1{t0 + 3600.};
const CenterOfMass com(mu, time0, Cartesian{{{-4500000., 4500000., 0.}}, {{0., 4000., 0.}}}, nullptr);
} // namespace

TEST_CASE("Elliptic construction")
{
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Elliptic);

    auto solver = UniversalKeplerSolver::create(com);
    REQUIRE(dynamic_cast<EllipticKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time1);
    const double guess = solver->initialGuess();
    CHECK(guess == Catch::Approx(-0.000371181));
    CHECK(solver->f(guess) == Catch::Approx(-2299.8885155579));
    CHECK(solver->df(guess) == Catch::Approx(2798379.6021787832));

    CHECK(root == Catch::Approx(0.0002836762));
    CHECK(solver->f(root) == Catch::Approx(0.));
    CHECK(solver->df(root) == Catch::Approx(1275954.2647539806));

    const auto factors = solver->factorsAt(guess);
    CHECK(factors.f == Catch::Approx(0.0028717009));
    CHECK(factors.g == Catch::Approx(696.3567775155));
    CHECK(factors.df == Catch::Approx(-0.001441273));
    CHECK(factors.dg == Catch::Approx(-1.2676286066));
    CHECK(factors.f * factors.dg - factors.df * factors.g == Catch::Approx(1.));

    const auto rootFactors = solver->factorsAt(root);
    CHECK(rootFactors.f == Catch::Approx(-0.137545));
    CHECK(rootFactors.g == Catch::Approx(433.682));
    CHECK(rootFactors.df == Catch::Approx(-0.000823565));
    CHECK(rootFactors.dg == Catch::Approx(-4.67363));
    CHECK(rootFactors.f * rootFactors.dg - rootFactors.df * rootFactors.g == Catch::Approx(1.));

    // Period results in same guess
    CHECK(com.orbitalPeriod().low() == 0.);
    CHECK(com.orbitalPeriod().high() == Catch::Approx(2192.6936220732));
    CHECK(root == Catch::Approx(solver->solveForInternal(time1 - com.orbitalPeriod().high())));
    CHECK(root == Catch::Approx(solver->solveForInternal(time1 + com.orbitalPeriod().high())));
}

TEST_CASE("Elliptic orbit")
{
    // Prepare solutions at a few selected times
    REQUIRE(com.orbitType() == CenterOfMass::OrbitType::Elliptic);

    const std::vector<double> t = {
        -1140.,
        -950.,
        -760.,
        -570.,
        -380.,
        -190.,
        0.,
        190.,
        380.,
        570.,
        760.,
        950.,
    };

    test::Results expected;
    expected.guess = {
        -0.0003531066,
        -0.0003630495,
        -0.0003723992,
        -0.0003812406,
        -0.0003896402,
        -0.0003976516,
        -0.0002790744,
        -0.0002962006,
        -0.0003110625,
        -0.0003243185,
        -0.0003363615,
        -0.0003474478,
    };
    expected.s = {
        0.0001658199,
        0.0002113948,
        0.0003085121,
        0.0004815033,
        0.0005325769,
        5.695951e-4, // 7.08723e-5,
        1.6552983919e-25,
        2.88719e-5,
        5.66892e-5,
        8.46126e-5,
        0.0001138397,
        0.0001460822,
    };
    expected.x = {
        -1.50311e6,
        -382280,
        649980,
        -2.73398e6,
        -3.86643e6,
        -4.36154e6,
        -4.5e6,
        -4.38341e6,
        -4.06021e6,
        -3.55515e6,
        -2.87926e6,
        -2.03343e6,
    };
    expected.y = {
        4.67181e6,
        3.38899e6,
        462911,
        874139,
        2.4297e6,
        3.60979e6,
        4.5e6,
        5.13705e6,
        5.53264e6,
        5.68035e6,
        5.55406e6,
        5.09642e6,
    };
    expected.vx = {
        5421.75,
        6346.41,
        -2812.31,
        -8914.55,
        -3876.04,
        -1539.35,
        7.3311407e-18,
        1186.82,
        2194.38,
        3112.65,
        4001.25,
        4909.27,
    };
    expected.vy = {
        -4876.14,
        -9176.34,
        -29696,
        9434.07,
        7091.19,
        5401.01,
        4000,
        2715.52,
        1443.1,
        89.7425,
        -1466.76,
        -3452.14,
    };

    auto solver = UniversalKeplerSolver::create(com);

    const auto c0 = com.initialCoordinates();
    const double h0 = c0.position().cross(c0.velocity())[2].value();

    test::Results().computeAndCheck(*solver, t, expected, h0);
}

TEST_CASE("Circular construction")
{
    constexpr double mu2{4e14};
    constexpr double r = 1e6;
    const double v = sqrt(mu2 * r) / r;
    const CenterOfMass circular(GravitationalParam{mu2}, time1, Cartesian{{{0., r, 0.}}, {{v, 0., 0.}}}, nullptr);
    CHECK(circular.orbitalElements().eccentricity_.value() == 0.);
    CHECK(circular.orbitalElements().alpha_.value() == 1. / r);
    CHECK(circular.orbitalElements().inclination_.value() == 0.);
    CHECK(circular.orbitalElements().longitude_.value() == 0.);
    CHECK(circular.orbitalElements().periapsis_.value() == 0.);
    REQUIRE(circular.orbitType() == CenterOfMass::OrbitType::Circular);

    auto solver = UniversalKeplerSolver::create(circular);
    REQUIRE(dynamic_cast<EllipticKeplerSolver*>(solver.get()) != nullptr);

    const double root = solver->solveForInternal(time0);
    const double guess = solver->initialGuess();
    CHECK(root == Catch::Approx(guess));
    CHECK(root == Catch::Approx(0.0001699112));
    CHECK(solver->f(root) == Catch::Approx(0.0));
    CHECK(solver->df(root) == Catch::Approx(1000000.0));

    const auto factors = solver->factorsAt(root);
    CHECK(factors.f == Catch::Approx(-0.9672505883));
    CHECK(factors.g == Catch::Approx(-12.6911681381));
    CHECK(factors.df == Catch::Approx(0.0050764673));
    CHECK(factors.dg == Catch::Approx(-0.9672505883));
    CHECK(factors.f * factors.dg - factors.df * factors.g == Catch::Approx(1.));

    // Period results in same guess
    CHECK(circular.orbitalPeriod().low() == 0.);
    CHECK(circular.orbitalPeriod().high() == Catch::Approx(314.159265359));
    CHECK(root == Catch::Approx(solver->solveForInternal(time0 - circular.orbitalPeriod().high())));
    CHECK(root == Catch::Approx(solver->solveForInternal(time0 + circular.orbitalPeriod().high())));

    // Norms are always the same
    for (double t = 0; t < circular.orbitalPeriod().high(); t += circular.orbitalPeriod().high() / 8.)
    {
        const auto coords = solver->coordinatesAt(qty::Second(time1 + t));
        const auto factors = solver->factorsAt(solver->computedS());
        CHECK(factors.f * factors.dg - factors.df * factors.g == Catch::Approx(1.));
        CHECK(coords.normPosition() == Catch::Approx(r));
        CHECK(coords.normVelocity() == Catch::Approx(v));
    }
}
