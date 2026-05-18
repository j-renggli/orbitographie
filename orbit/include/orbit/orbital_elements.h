#pragma once

#include <math/bounded_quantity.h>
#include <math/quantity.h>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;
namespace angle = qty::angle;

using Eccentricity = qty::BoundedQuantity<double, math::unit::Unitless, 0.>;
using SemiMajorAxis = qty::PerMetre;

class OrbitalElements
{
public:
    /// Construct orbital elements and also provide the standard gravitational
    /// parameter this orbit is centered around to precompute the orbital period
    OrbitalElements(const Eccentricity& eccentricity,
                    const SemiMajorAxis& alpha, // 1 / semi-major axis
                    const angle::HalfRadian& inclination,
                    const angle::FullRadian& longitude,
                    const angle::FullRadian& periapsis);

    /// Eccentricity: strictly positive
    Eccentricity eccentricity_;

    /// 1 / Semi-major axis: positive for e < 1, undefined for e = 1, negative for e > 1
    SemiMajorAxis alpha_;

    /// Inclination: in range [0, pi]
    angle::HalfRadian inclination_;

    /// Longitude of ascending node: in range [0, 2*pi)
    angle::FullRadian longitude_;

    /// Argument of periapsis: in range [0, 2*pi)
    angle::FullRadian periapsis_;
};

} // namespace orbit
} // namespace galaxias
