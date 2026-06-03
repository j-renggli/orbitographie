#pragma once

#include <math/bounded_quantity.h>

#include <eigen3/Eigen/Dense>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;

using Vector = Eigen::Vector3d;

namespace coordinates
{

class Cartesian
{
public:
    using Position = qty::Quantity<Vector, math::unit::Metre>;
    using Velocity = qty::Quantity<Vector, math::unit::Velocity>;
    using SAM = qty::Quantity<Vector, math::unit::Unit<math::unit::ratio::NegOne, math::unit::ratio::Two>>;

    Cartesian(const Position& position, const Velocity& velocity);
    static Cartesian zero();

    const Position& position() const { return r_; }
    double normPosition() const { return r_.value().norm(); }
    const Velocity& velocity() const { return v_; }
    double normVelocity() const { return v_.value().norm(); }
    const SAM specificAngularMomentum() const { return r_.cross(v_); }

    friend std::ostream& operator<<(std::ostream& out, const Cartesian& cartesian);

private:
    Position r_;
    Velocity v_;
};

class StelloCentric : public Cartesian
{
};

class Perifocal : public Cartesian
{
};

class GalactoCentric
{
public:
    GalactoCentric(const qty::angle::FullRadian& angle, const qty::positive::Metre& radius, const qty::Metre& height);

    const qty::angle::FullRadian& angle() const { return angle_; }
    const qty::positive::Metre& radius() const { return radius_; }
    const qty::Metre& height() const { return height_; }

    friend std::ostream& operator<<(std::ostream& out, const GalactoCentric& cartesian);

private:
    const qty::angle::FullRadian angle_;
    const qty::positive::Metre radius_;
    const qty::Metre height_;
};

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
