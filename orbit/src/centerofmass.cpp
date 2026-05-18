#include <orbit/centerofmass.h>

#include "keplersolver/solver.h"

namespace galaxias
{
namespace orbit
{

using namespace math;
using namespace unit;
using namespace coordinates;

namespace
{

using NoUnit = qty::Quantity<double, unit::Unit<>>;
using SquaredVelocity = qty::Quantity<double, MultiplyUnit<MetreSquared, FrequencySquared>::value_type>;
using UnitlessVector = qty::Quantity<Vector, Unitless>;
using UnitMMS = MultiplyUnit<MetreSquared, Frequency>::value_type;

constexpr double two_pi{2. * M_PI};

OrbitalElements
deduceElements(const GravitationalParam& centralMu, const Cartesian::Position& r0, const Cartesian::Velocity& v0)
{
    // h = r x v
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
    const qty::Radian i{acos((hVec[2] / hVec.norm()).value())};

    // n = K x h
    const qty::Quantity<Vector, UnitMMS> nVec{{-hVec.value()[1], hVec.value()[0], 0.}};
    const qty::Quantity<double, UnitMMS> n = nVec.norm();

    // Longitude in [0, 2pi]
    const qty::Radian longitude = n == 0. ? 0. : (nVec[1] >= 0. ? 1. : -1.) * acos((nVec[0] / n).value());

    // Argument of periapsis in [0, 2pi] or here [-pi,pi]
    const auto ndote = nVec.dot(eVec);
    double arg = (eVec.value()[2] >= 0. ? 1. : -1.) * acos((ndote / (n * e)).value());
    if (ndote == 0.)
    {
        // For ellipsis, the argument does not matter so can be 0. Otherwise use the value from longitude of periapsis
        arg = e == 0 ? 0. : acos((eVec[0] / e).value());
    }

    return OrbitalElements{e, alpha, i, longitude, arg};
}

} // namespace

CenterOfMass::CenterOfMass(const GravitationalParam& mu)
    : CenterOfMass{mu, 0., coordinates::Cartesian::zero(), nullptr}
{
}

CenterOfMass::CenterOfMass(const GravitationalParam& mu,
                           const qty::Second& time0,
                           const coordinates::Cartesian& coord0,
                           const std::shared_ptr<CenterOfMass>& parent)
    : mu_{mu}
    , t0_{time0}
    , coord0_{coord0}
    , oe_{deduceElements(parent ? parent->mu_ : mu_, coord0_.position(), coord0_.velocity())}
    , parent_{parent}
{
    if (oe_.eccentricity_ == 0.)
    {
        orbitType_ = OrbitType::Circular;
    }
    else if (oe_.eccentricity_ < 1.)
    {
        orbitType_ = OrbitType::Elliptic;
    }
    else if (oe_.eccentricity_ > 1.)
    {
        orbitType_ = OrbitType::Hyperbolic;
    }
    else if (coord0_.position().cross(coord0_.velocity()).squaredNorm() == 0.)
    {
        orbitType_ = OrbitType::Degenerate;
    }
    else
    {
        orbitType_ = OrbitType::Parabolic;
    }
}

CenterOfMass::~CenterOfMass() = default;

math::Range<double> CenterOfMass::orbitalPeriod() const
{
    if (oe_.eccentricity_ >= 1.)
    {
        throw std::runtime_error("Orbital period is only defined for elliptical (circular) case");
    }

    const qty::Frequency freq{(oe_.alpha_.pow<3>() * (parent_ ? parent_->mu_ : mu_)).root<2>()};
    return math::Range<double>{0., two_pi / freq.value()};
}

} // namespace orbit
} // namespace galaxias
