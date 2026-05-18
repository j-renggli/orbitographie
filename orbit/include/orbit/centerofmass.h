#pragma once

#include "orbital_elements.h"
#include "position.h"
#include <math/bounded_quantity.h>

#include <memory>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;

using GravitationalParam =
    qty::BoundedQuantity<double, math::unit::Unit<math::unit::ratio::NegTwo, math::unit::ratio::Three>, 0.>;

class CenterOfMass
{
public:
    CenterOfMass();
    CenterOfMass(const CenterOfMass&) = default;
    CenterOfMass(CenterOfMass&&) noexcept = default;
    CenterOfMass(const GravitationalParam& mu);
    CenterOfMass(const GravitationalParam& mu,
                 const qty::Second& time0,
                 const coordinates::Cartesian& coord0,
                 const std::shared_ptr<CenterOfMass>& parent);
    CenterOfMass(const GravitationalParam& mu,
                 const qty::Second& time0,
                 const OrbitalElements& oe,
                 const std::shared_ptr<CenterOfMass>& parent);
    virtual ~CenterOfMass();

    CenterOfMass& operator=(const CenterOfMass&) = default;
    CenterOfMass& operator=(CenterOfMass&&) noexcept = default;

    const GravitationalParam& mu() const { return mu_; }
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
    qty::Second t0_;
    coordinates::Cartesian coord0_;
    OrbitalElements oe_;
    OrbitType orbitType_;
    std::shared_ptr<CenterOfMass> parent_;
};

} // namespace orbit
} // namespace galaxias
