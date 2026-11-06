#pragma once

#include <math/bounded_quantity.h>

#include <eigen3/Eigen/Dense>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;

using Vector = Eigen::Vector3d;

namespace position
{

using Cartesian = qty::Quantity<Vector, math::unit::Metre>;

class Cylindrical
{
public:
    Cylindrical(const qty::positive::Metre& radius_ = 0.,
                const qty::angle::FullRadian& angle_ = 0.,
                const qty::Metre& height_ = 0.)
        : radius{radius_}
        , angle{angle_}
        , height{height_}
    {
    }
    Cylindrical(const Cylindrical&) = default;
    Cylindrical(Cylindrical&&) noexcept = default;
    Cylindrical(const Cartesian& cartesian)
        : radius{sqrt(cartesian.value().x() * cartesian.value().x() + cartesian.value().y() * cartesian.value().y())}
        , angle{qty::angle::FullRadian::range().modulo(std::atan2(cartesian.value().y(), cartesian.value().x()))}
        , height{cartesian.value().z()}
    {
    }
    ~Cylindrical() = default;

    Cylindrical& operator=(const Cylindrical&) = default;
    Cylindrical& operator=(Cylindrical&&) = default;

    Cartesian toCartesian() const
    {
        const double r = radius.value();
        return {{r * cos(angle.value()), r * sin(angle.value()), height.value()}};
    }

    qty::positive::Metre radius;
    qty::angle::FullRadian angle;
    qty::Metre height;
};

} // namespace position

namespace velocity
{

using Cartesian = qty::Quantity<Vector, math::unit::Velocity>;

} // namespace velocity

namespace coordinates
{

class Cartesian
{
public:
    using SAM = qty::Quantity<Vector, math::unit::Unit<math::unit::ratio::NegOne, math::unit::ratio::Two>>;

    using Position = position::Cartesian;
    using Velocity = velocity::Cartesian;

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
