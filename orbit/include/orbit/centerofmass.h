#pragma once

#include "orbital_elements.h"
#include "position.h"
#include "quantities.h"
#include "universal_kepler_solver.h"
#include <math/bounded_quantity.h>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;

class CenterOfMass
{
public:
    CenterOfMass() = delete;
    CenterOfMass(const CenterOfMass&) = default;
    CenterOfMass(CenterOfMass&&) noexcept = default;
    CenterOfMass(const GravitationalParam& mu);
    CenterOfMass(const GravitationalParam& mu,
                 const qty::Second& time0,
                 const coordinates::Cartesian& coord0,
                 std::optional<GravitationalParam> parent = std::nullopt);
    CenterOfMass(const GravitationalParam& mu,
                 const qty::Second& time0,
                 const OrbitalElements& oe,
                 std::optional<GravitationalParam> parent = std::nullopt);

    virtual ~CenterOfMass();

    CenterOfMass& operator=(const CenterOfMass&) = default;
    CenterOfMass& operator=(CenterOfMass&&) noexcept = default;

    const GravitationalParam& gravitationalParam() const { return mu_; }
    const qty::Second& initialTime() const { return t0_; }

    const coordinates::Cartesian& initialCoordinates() const { return coord0_; }
    const coordinates::Cartesian::Position& initialPosition() const { return coord0_.position(); }
    const coordinates::Cartesian::Velocity& initialVelocity() const { return coord0_.velocity(); }

    const OrbitalElements& orbitalElements() const { return oe_; }

    /// Helper function for elliptic case. Throws in all other cases
    math::Range<double> orbitalPeriod() const;

    enum class OrbitType
    {
        Circular,
        Elliptic,
        Parabolic,
        Hyperbolic,
        Degenerate,
    };
    OrbitType orbitType() const { return orbitType_; }

private:
    GravitationalParam mu_;
    GravitationalParam parentMu_;
    qty::Second t0_;
    coordinates::Cartesian coord0_;
    OrbitalElements oe_;
    OrbitType orbitType_;
};

} // namespace orbit
} // namespace galaxias
