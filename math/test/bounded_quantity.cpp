#include <math/bounded_quantity.h>

#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;
using namespace quantity;

namespace
{
constexpr double min{-std::numeric_limits<double>::max()};
constexpr double max{std::numeric_limits<double>::max()};
} // namespace

TEST_CASE("No bounds")
{
    using FullUnitless = BoundedQuantity<double, unit::Unitless, min, max>;
    const FullUnitless x{0.};
    CHECK(x.value() == 0.);
    CHECK(FullUnitless::range().low() == min);
    CHECK(FullUnitless::range().high() == max);

    CHECK_NOTHROW(FullUnitless(min));
    CHECK_NOTHROW(FullUnitless(max));
}

TEST_CASE("Minimum")
{
    using PositiveUnitless = BoundedQuantity<double, unit::Unitless, 0.>;
    CHECK(PositiveUnitless::range().low() == 0.);
    CHECK(PositiveUnitless::range().high() == max);

    CHECK_THROWS_AS(PositiveUnitless(-1.), std::runtime_error);
    CHECK_NOTHROW(PositiveUnitless(max));
}

TEST_CASE("Maximum")
{
    using NegativeUnitless = BoundedQuantity<double, unit::Unitless, -std::numeric_limits<double>::max(), 0.>;
    CHECK(NegativeUnitless::range().low() == min);
    CHECK(NegativeUnitless::range().high() == 0.);

    CHECK_NOTHROW(NegativeUnitless(min));
    CHECK_THROWS_AS(NegativeUnitless(1.), std::runtime_error);
}

TEST_CASE("Both")
{
    constexpr double zero{0.};
    constexpr double one{1.};
    using OneUnitless = BoundedQuantity<double, unit::Unitless, -one, one>;
    CHECK(OneUnitless::range().low() == -1.);
    CHECK(OneUnitless::range().high() == 1.);

    CHECK_THROWS_AS(OneUnitless(-2.), std::runtime_error);
    CHECK_NOTHROW(OneUnitless(0.));
    CHECK_THROWS_AS(OneUnitless(2.), std::runtime_error);

    using ZeroUnitless = BoundedQuantity<double, unit::Unitless, zero, zero>;
    CHECK_THROWS_AS(ZeroUnitless(0.), std::runtime_error);
    using NegUnitless = BoundedQuantity<double, unit::Unitless, one, -one>;
    CHECK_THROWS_AS(NegUnitless(1.), std::runtime_error);
}
