#pragma once

#include <math/quantity.h>
#include <math/solver/solver.h>

#include <memory>

namespace galaxias
{
namespace orbit
{

namespace coordinates
{

class Cartesian;

}

class CenterOfMass;
class OrbitalElements;

class IUniversalKeplerSolver : public math::solver::IFunction
{
public:
    virtual ~IUniversalKeplerSolver() {}

    /// Create a new solver from the given center of mass
    static std::unique_ptr<IUniversalKeplerSolver> create(CenterOfMass&& com);

    /// Return the coordinates at time t
    virtual coordinates::Cartesian coordinatesAt(const math::quantity::Second& targetTime) = 0;

    /// Return the center of mass
    virtual const CenterOfMass& centerOfMass() const = 0;
};

} // namespace orbit
} // namespace galaxias
