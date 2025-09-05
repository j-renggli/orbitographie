#include <math/derived_quantity.h>

#include <math/bounded_quantity.h>

#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;
using namespace quantity;

namespace
{

constexpr Range<double> pm_hundred{-100., 100.};

constexpr double factor{3.14159};
constexpr double value{5.};

class MyQty : public DerivedQuantity<math::quantity::Metre>
{
public:
    MyQty(double x)
        : DerivedQuantity(x, factor)
    {
    }
};

class MyBoundedQty : public DerivedQuantity<BoundedQuantity<double, math::unit::Unit<math::unit::ratio::One>>>
{
public:
    MyBoundedQty(double x)
        : DerivedQuantity(x, factor, pm_hundred)
    {
    }
};

} // namespace

TEST_CASE("Factor is taken into account")
{
    const MyQty x{value};
    CHECK(x.value() == value);
    CHECK(x.base().value() == value * factor);
}

TEST_CASE("Range is taken into account")
{
    constexpr double value{5.};
    const MyBoundedQty x{value};
    CHECK(x.value() == value);
    CHECK(x.base().value() == value * factor);
    CHECK(x.base().range() == pm_hundred);
}
