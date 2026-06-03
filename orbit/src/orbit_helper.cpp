#include "orbit_helper.h"

#include <math/unit.h>

namespace galaxias
{
namespace orbit
{

using namespace math;
using namespace unit;
using namespace coordinates;

namespace
{

using SquaredVelocity = qty::Quantity<double, MultiplyUnit<MetreSquared, FrequencySquared>::value_type>;
using UnitMMS = MultiplyUnit<MetreSquared, Frequency>::value_type;

} // namespace

OrbitalElements stateVectorsToElements(const coordinates::Cartesian& stateVectors, const GravitationalParam& centralMu)
{
    // Checked with https://elainecoe.github.io/orbital-mechanics-calculator/calculator.html

    // h = r x v
    const auto r0 = stateVectors.position();
    const auto v0 = stateVectors.velocity();
    const qty::Quantity<Vector, UnitMMS> hVec{r0.value().cross(v0.value())};
    const qty::Quantity<double, MultiplyUnit<UnitMMS, UnitMMS>::value_type> h2{hVec.value().squaredNorm()};
    if (centralMu == 0. || h2 == 0.)
    {
        // Degenerate conic that is either a point or a line
        // TODO: Apply perturbation so as to fall back to hyperbolic? Use e = inf, a = -inf?
        // NO, h = 0, e = 1  is what a point / straight line must be!
        if (r0.squaredNorm().value() + v0.squaredNorm().value() > 0.)
        {
            throw std::runtime_error("Linear case not implemented");
        }
        return OrbitalElements{1., 0., 0., 0., 0.};
    }

    const qty::Metre r = r0.norm(); // > 0 by if clause above
    const SquaredVelocity v2 = v0.squaredNorm();
    const qty::Quantity<double, UnitMMS> rdotv{r0.dot(v0)};

    // e = ((v^2 - mu / r) * R - (R * V) * V) mu
    const auto eVec = (r0 * (v2 - centralMu / r) - v0 * rdotv) / centralMu;
    const auto e{eVec.norm()};

    // alpha = 1 / semi-major axis so it's never 0
    const qty::Metre semiLatusRectum = h2 / centralMu;
    const qty::PerMetre alpha = (-e * e + 1.) / semiLatusRectum;

    // Inclination i in [0, pi]
    const angle::HalfRadian i{acos((hVec[2] / hVec.norm()).value())};

    // n = K x h
    const qty::Quantity<Vector, UnitMMS> nVec{{-hVec.value()[1], hVec.value()[0], 0.}};
    const qty::Quantity<double, UnitMMS> n = nVec.norm();

    // Longitude in [0, 2pi]
    double longitude = n == 0. ? 0. : (nVec[1] >= 0. ? 1. : -1.) * acos((nVec[0] / n).value());
    Range<double>::radians().modulo(longitude);

    // Argument of periapsis in [0, 2pi] or here [-pi,pi]
    const auto ndote = nVec.dot(eVec);
    double arg = (eVec.value()[2] >= 0. ? 1. : -1.) * acos((ndote / (n * e)).value());
    if (ndote == 0.)
    {
        // For ellipsis, the argument does not matter so can be 0. Otherwise use the value from longitude of periapsis
        arg = e == 0 ? 0. : acos((eVec[0] / e).value());
    }
    Range<double>::radians().modulo(arg);

    return OrbitalElements{e, alpha, i, longitude, arg};
}

coordinates::Cartesian elementsToStateVectors(const OrbitalElements& oe, const GravitationalParam& parentMu)
{
    // See https://downloads.rene-schwarz.com/download/M001-Keplerian_Orbit_Elements_to_Cartesian_State_Vectors.pdf
    // Checked with https://www2.arnes.si/~gljsentvid10/ele2vec.html
    // Visualised with https://alfonsogonzalez.github.io/AWP/

    // Position at t0 in orbital plane
    // M0 = 0 => E = 0 => nu = 0
    const qty::Metre ox = (qty::Unitless{1.} - oe.eccentricity_) / oe.alpha_;

    const qty::Velocity vel = (parentMu / oe.alpha_).root<2>() / ox;
    const qty::Unitless e = (qty::Unitless{1} - oe.eccentricity_.pow<2>()).root<2>();
    const qty::Velocity dy = vel * e;

    const double ci = cos(oe.inclination_.value());
    const double si = sin(oe.inclination_.value());
    const double co = cos(oe.longitude_.value());
    const double so = sin(oe.longitude_.value());
    const double cw = cos(oe.periapsis_.value());
    const double sw = sin(oe.periapsis_.value());

    const coordinates::Cartesian::Position p = {
        {ox.value() * (cw * co - sw * ci * so), ox.value() * (cw * so + sw * ci * co), ox.value() * (sw * si)}};
    const coordinates::Cartesian::Velocity v = {
        {-dy.value() * (sw * co + cw * ci * so), dy.value() * (cw * ci * co - sw * so), dy.value() * (cw * si)}};

    return coordinates::Cartesian{p, v};
}

CenterOfMass::OrbitType getOrbitType(const Eccentricity& eccentricity, const coordinates::Cartesian& coord0)
{
    if (eccentricity == 0.)
    {
        return CenterOfMass::OrbitType::Circular;
    }
    else if (eccentricity < 1.)
    {
        return CenterOfMass::OrbitType::Elliptic;
    }
    else if (eccentricity > 1.)
    {
        return CenterOfMass::OrbitType::Hyperbolic;
    }
    else if (coord0.position().cross(coord0.velocity()).squaredNorm() == 0.)
    {
        return CenterOfMass::OrbitType::Degenerate;
    }
    else
    {
        return CenterOfMass::OrbitType::Parabolic;
    }
}

} // namespace orbit
} // namespace galaxias
