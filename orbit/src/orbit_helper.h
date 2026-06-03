#pragma once

#include "orbit/centerofmass.h"
#include "orbit/orbital_elements.h"
#include "orbit/position.h"

namespace galaxias
{
namespace orbit
{

OrbitalElements stateVectorsToElements(const coordinates::Cartesian& stateVectors, const GravitationalParam& centralMu);

coordinates::Cartesian elementsToStateVectors(const OrbitalElements& oe, const GravitationalParam& parentMu);

CenterOfMass::OrbitType getOrbitType(const Eccentricity& eccentricity, const coordinates::Cartesian& coord0);

} // namespace orbit
} // namespace galaxias
