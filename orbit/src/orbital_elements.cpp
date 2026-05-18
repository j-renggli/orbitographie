#include <orbit/orbital_elements.h>

#include <math/solver/newton_raphson.h>

namespace galaxias
{
namespace orbit
{

OrbitalElements::OrbitalElements(const Eccentricity& eccentricity,
                                 const SemiMajorAxis& alpha,
                                 const angle::HalfRadian& inclination,
                                 const angle::FullRadian& longitude,
                                 const angle::FullRadian& periapsis)
    : eccentricity_{eccentricity}
    , alpha_{alpha}
    , inclination_{inclination}
    , longitude_{longitude}
    , periapsis_{periapsis}
{
}

} // namespace orbit
} // namespace galaxias
