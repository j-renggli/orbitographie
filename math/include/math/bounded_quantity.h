#pragma once

#include "quantity.h"
#include <math/range.h>

#include <stdexcept>

namespace galaxias
{
namespace math
{
namespace quantity
{

template <class T, class U>
struct BoundedQuantity : public Quantity<T, U>
{
    using Base = Quantity<T, U>;

    constexpr BoundedQuantity(const Quantity<T, U>& qty,
                              const Range<double>& range = Range<double>(-std::numeric_limits<double>::max(),
                                                                         std::numeric_limits<double>::max()))
        : Quantity<T, U>{qty}
        , range_{range}
    {
        if (!std::isnan(qty.value()) && !range_.includes(qty.value()))
        {
            throw std::runtime_error("Out of bounds " + std::to_string(range_.low()) +
                                     " <= " + std::to_string(this->value()) + " <= " + std::to_string(range_.high()));
        }
    }
    constexpr BoundedQuantity(const BoundedQuantity& bqty)
        : Quantity<T, U>{bqty}
        , range_{bqty.range_}
    {
    }
    constexpr BoundedQuantity(BoundedQuantity&& qty) = default;
    ~BoundedQuantity() = default;

    BoundedQuantity& operator=(const BoundedQuantity& rhs) = default;
    BoundedQuantity& operator=(BoundedQuantity&& rhs) = default;

    static BoundedQuantity fromModulo(const Quantity<T, U>& value, const Range<double>& range)
    {
        return BoundedQuantity{range.modulo(value.value()), range};
    }

    const Range<double>& range() const { return range_; }

private:
    Range<double> range_;
};

// Unitless
using BoundedUnitless = BoundedQuantity<double, unit::Unitless>;
using BoundedRadian = BoundedQuantity<double, unit::Unitless>;

// Time
using BoundedSecond = BoundedQuantity<double, unit::Second>;
using BoundedFrequency = BoundedQuantity<double, unit::Frequency>;

// Length
using BoundedMetre = BoundedQuantity<double, unit::Metre>;

// Mass
using BoundedKilogram = BoundedQuantity<double, unit::Kilogram>;

// Temperature
using BoundedKelvin = BoundedQuantity<double, unit::Kelvin>;

// Composite
using BoundedVelocity = BoundedQuantity<double, unit::Velocity>;
using BoundedWatt = BoundedQuantity<double, unit::Watt>;

} // namespace quantity
} // namespace math
} // namespace galaxias
