#include <orbit/position.h>

namespace galaxias
{
namespace orbit
{
namespace coordinates
{

Cartesian::Cartesian(const Position& position, const Velocity& velocity)
    : r_{position}
    , v_{velocity}
{
}

Cartesian Cartesian::zero()
{
    return Cartesian{Position{Position::value_type::Zero()}, Velocity{Velocity::value_type::Zero()}};
}

std::ostream& operator<<(std::ostream& out, const Cartesian& cartesian)
{
    const auto& r = cartesian.r_.value();
    const auto& v = cartesian.v_.value();
    out << "r=[" << r[0] << "," << r[1] << "," << r[2] << "] m, v=[" << v[0] << "," << v[1] << "," << v[2] << "] m/s\n";
    return out;
}

GalactoCentric::GalactoCentric(const qty::angle::FullRadian& angle,
                               const qty::positive::Metre& radius,
                               const qty::Metre& height)
    : angle_{angle}
    , radius_{radius}
    , height_{height}
{
}

} // namespace coordinates
} // namespace orbit
} // namespace galaxias
