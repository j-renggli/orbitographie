#pragma once

#include <math/bounded_quantity.h>

namespace galaxias
{
namespace orbit
{

namespace qty = math::quantity;

using GravitationalParam =
    qty::BoundedQuantity<double, math::unit::Unit<math::unit::ratio::NegTwo, math::unit::ratio::Three>, 0.>;

} // namespace orbit
} // namespace galaxias
