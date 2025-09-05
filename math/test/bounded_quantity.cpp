#include <math/bounded_quantity.h>

#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;
using namespace quantity;

namespace
{
constexpr double min{-std::numeric_limits<double>::max()};
constexpr double max{std::numeric_limits<double>::max()};
constexpr Range<double> pm_one{-1., 1.};
} // namespace

TEST_CASE("No bounds")
{
    const BoundedUnitless x{0.};
    CHECK(x.value() == 0.);
    CHECK(x.range().low() == min);
    CHECK(x.range().high() == max);

    CHECK_NOTHROW(BoundedUnitless(min));
    CHECK_NOTHROW(BoundedUnitless(max));
}

TEST_CASE("Minimum")
{
    const BoundedUnitless x{0., Range<double>::positive()};
    CHECK(x.range().low() == 0.);
    CHECK(x.range().high() == max);

    CHECK_THROWS_AS(BoundedUnitless(-1., Range<double>::positive()), std::runtime_error);
    CHECK_NOTHROW(BoundedUnitless(max, Range<double>::positive()));
}

TEST_CASE("Maximum")
{
    const BoundedUnitless x{0., Range<double>::negative()};
    CHECK(x.range().low() == min);
    CHECK(x.range().high() == 0.);

    CHECK_NOTHROW(BoundedUnitless(min, Range<double>::negative()));
    CHECK_THROWS_AS(BoundedUnitless(1., Range<double>::negative()), std::runtime_error);
}

TEST_CASE("Both")
{
    const BoundedUnitless x{0., pm_one};
    CHECK(x.range().low() == -1.);
    CHECK(x.range().high() == 1.);

    CHECK_THROWS_AS(BoundedUnitless(-2., pm_one), std::runtime_error);
    CHECK_NOTHROW(BoundedUnitless(0., pm_one));
    CHECK_THROWS_AS(BoundedUnitless(2., pm_one), std::runtime_error);

    CHECK_THROWS_AS(BoundedUnitless(0., {0., 0.}), std::runtime_error);
    CHECK_THROWS_AS(BoundedUnitless(0., {1., -1.}), std::runtime_error);
}
