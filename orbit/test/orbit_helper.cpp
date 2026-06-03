#include "../src/orbit_helper.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace galaxias;
using namespace orbit;
using namespace math;
using namespace quantity;
using namespace coordinates;

namespace
{

void checkOrbitalElements(const OrbitalElements& elements,
                          double eccentricity,
                          double alpha,
                          double inclination,
                          double longitude,
                          double periapsis)
{
    CHECK(elements.eccentricity_.value() == Catch::Approx(eccentricity));
    CHECK(elements.alpha_.value() * 1e8 == Catch::Approx(alpha * 1e8));
    CHECK(elements.inclination_.value() == Catch::Approx(inclination));
    CHECK(elements.longitude_.value() == Catch::Approx(longitude));
    CHECK(elements.periapsis_.value() == Catch::Approx(periapsis));
}

void compare(const Cartesian& input, const Cartesian& output)
{
    const auto samIn = input.specificAngularMomentum();
    const auto samOut = output.specificAngularMomentum();

    CHECK_THAT(samIn.value().x(), Catch::Matchers::WithinAbs(samOut.value().x(), 5e-6));
    if (std::abs(samIn.value().y()) > 1000.)
    {
        CHECK_THAT(samIn.value().y(), Catch::Matchers::WithinRel(samOut.value().y(), 5e-6));
    }
    else
    {
        CHECK_THAT(samIn.value().y(), Catch::Matchers::WithinAbs(samOut.value().y(), 5e-6));
    }
    CHECK_THAT(samIn.value().z(), Catch::Matchers::WithinAbs(samOut.value().z(), 5e-6));
}

} // namespace

TEST_CASE("Orbit type")
{
    const coordinates::Cartesian::Position p0{{0., 0., 0.}};
    const coordinates::Cartesian::Velocity v0{{0., 0., 0.}};
    const coordinates::Cartesian::Position p1{{1., 0., 0.}};
    const coordinates::Cartesian::Velocity v1{{0., 1., 0.}};

    CHECK(getOrbitType(0., {p0, v0}) == CenterOfMass::OrbitType::Circular);
    CHECK(getOrbitType(0.5, {p0, v0}) == CenterOfMass::OrbitType::Elliptic);
    CHECK(getOrbitType(1.5, {p0, v0}) == CenterOfMass::OrbitType::Hyperbolic);
    CHECK(getOrbitType(1., {p0, v0}) == CenterOfMass::OrbitType::Degenerate);
    CHECK(getOrbitType(1., {p1, v1}) == CenterOfMass::OrbitType::Parabolic);
}

TEST_CASE("Conversions")
{
    const GravitationalParam mu{3.986004418e14};
    {
        INFO("Example 1");
        const Cartesian sv{{{12756200., 0., 0.}}, {{0., 7905.37, 0.}}};
        const auto oe = stateVectorsToElements(sv, mu);
        checkOrbitalElements(oe, 0.999990564, 7.39721e-13, 0., 0., 0.);
        const auto back = elementsToStateVectors(oe, mu);
        compare(sv, back);
    }
    {
        INFO("Example 2");
        const Cartesian sv{{{8750000., 5100000., 0.}}, {{-3000., 5200., 5900.}}};
        const auto oe = stateVectorsToElements(sv, mu);
        checkOrbitalElements(oe, 0.8001889565, 1.9729174678e-8, 0.776722, 0.527719, 6.2760);
        const auto back = elementsToStateVectors(oe, mu);
        compare(sv, back);
    }
    {
        INFO("Example 3");
        constexpr double x = 4500000.;
        const Cartesian sv{{{-x, x, 0.}}, {{0., 4000., 0.}}};
        const auto oe = stateVectorsToElements(sv, mu);
        checkOrbitalElements(oe, 0.881576, 0.274129233297e-6, M_PI, 0., 0.9307937946);
        const auto back = elementsToStateVectors(oe, mu);
        compare(sv, back);
    }
    {
        INFO("Example 4");
        const Cartesian sv{{{0., 0., 7653760.}}, {{3162., 0., -2371.6}}};
        const auto oe = stateVectorsToElements(sv, mu);
        checkOrbitalElements(oe, 0.820748, 0.222115512059e-6, 0.5 * M_PI, M_PI, 4.5360354435);
        const auto back = elementsToStateVectors(oe, mu);
        INFO("p0 = " << sv.position().value());
        INFO("p1 = " << back.position().value());
        INFO("v0 = " << sv.velocity().value());
        INFO("v1 = " << back.velocity().value());
        compare(sv, back);
    }
    {
        INFO("Example 5");
        constexpr double x = 6378140.;
        constexpr double y = 2632.34;
        const Cartesian sv{{{-x, -x, -x}}, {{y, -y, y}}};
        const auto oe = stateVectorsToElements(sv, mu);
        checkOrbitalElements(oe, 0.52039638, 0.1288887e-6, 0.7853981634, 4.7123889804, 1.8309151074);
        const auto back = elementsToStateVectors(oe, mu);
        compare(sv, back);
    }
}
