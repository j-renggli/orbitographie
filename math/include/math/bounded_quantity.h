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

template <class T, class U, T min, T max = std::numeric_limits<T>::max()>
struct BoundedQuantity : public Quantity<T, U>
{
    using Base = Quantity<T, U>;
    static constexpr T limitMin = min;
    static constexpr T limitMax = max;

    constexpr BoundedQuantity(const T& value)
        : Quantity<T, U>{value}
        , range_{limitMin, limitMax}
    {
        if (!std::isnan(value) && !range_.includes(value))
        {
            throw std::runtime_error("Out of bounds " + std::to_string(range_.low()) +
                                     " <= " + std::to_string(this->value()) + " <= " + std::to_string(range_.high()));
        }
    }
    constexpr BoundedQuantity(const Quantity<T, U>& qty)
        : Quantity<T, U>{qty}
        , range_{limitMin, limitMax}
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

    static Range<T> range() { return Range<T>{limitMin, limitMax}; }

private:
    Range<T> range_;
};

// Quantities can often only take positive values
namespace positive
{

using Second = BoundedQuantity<double, unit::Second, 0.>;
using Metre = BoundedQuantity<double, unit::Metre, 0.>;
using Kilogram = BoundedQuantity<double, unit::Kilogram, 0.>;
using Watt = BoundedQuantity<double, unit::Watt, 0.>;
using Kelvin = BoundedQuantity<double, unit::Kelvin, 0.>;

} // namespace positive

// Angles only have meaning between specific values, since they are cyclic
namespace angle
{

using FullRadian = BoundedQuantity<double, unit::Unitless, 0., 2 * M_PI>;
using HalfRadian = BoundedQuantity<double, unit::Unitless, 0., M_PI>;
using PrincipalRadian = BoundedQuantity<double, unit::Unitless, -M_PI, M_PI>;

} // namespace angle

} // namespace quantity
} // namespace math
} // namespace galaxias
