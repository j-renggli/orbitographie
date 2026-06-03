#include <orbit/centerofmass.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace orbit;
using namespace math;
using namespace quantity;
using namespace coordinates;

namespace
{

constexpr double pi{M_PI};

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

} // namespace

TEST_CASE("Center of mass")
{
    //    constexpr qty::Kilogram earthMass{5.972179854e24};
    const GravitationalParam mu{3.986004418e14};
    constexpr qty::Second time0{0.};
    {
        INFO("Example 1");
        CenterOfMass com(mu, time0, Cartesian{{{12756200., 0., 0.}}, {{0., 7905.37, 0.}}}, nullptr);
        checkOrbitalElements(com.orbitalElements(), 0.999990564, 7.39721e-13, 0., 0., 0.);
    }
    {
        INFO("Example 2");
        CenterOfMass com(mu, time0, Cartesian{{{8750000., 5100000., 0.}}, {{-3000., 5200., 5900.}}}, nullptr);
        checkOrbitalElements(com.orbitalElements(), 0.8001889565, 1.9729174678e-8, 0.776722, 0.527719, 6.2760);
    }
    {
        INFO("Example 3");
        constexpr double x = 4500000.;
        CenterOfMass com(mu, time0, Cartesian{{{-x, x, 0.}}, {{0., 4000., 0.}}}, nullptr);
        checkOrbitalElements(com.orbitalElements(), 0.881576, 0.274129233297e-6, pi, 0., 0.9307937946);
    }
    {
        INFO("Example 4");
        CenterOfMass com(mu, time0, Cartesian{{{0., 0., 7653760.}}, {{3162., 0., -2371.6}}}, nullptr);
        checkOrbitalElements(com.orbitalElements(), 0.820748, 0.222115512059e-6, 0.5 * pi, pi, 4.5360354435);
    }
    {
        INFO("Example 5");
        constexpr double x = 6378140.;
        constexpr double y = 2632.34;
        CenterOfMass com(mu, time0, Cartesian{{{-x, -x, -x}}, {{y, -y, y}}}, nullptr);
        checkOrbitalElements(com.orbitalElements(), 0.52039638, 0.1288887e-6, 0.7853981634, 4.7123889804, 1.8309151074);
    }
}
