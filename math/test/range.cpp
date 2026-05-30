#include <math/range.h>

#include <catch2/catch_template_test_macros.hpp>

#include <stdexcept>

using namespace galaxias;
using namespace math;

namespace
{

} // namespace

TEMPLATE_TEST_CASE("Invalid range", "[range]", int, double)
{
    CHECK_THROWS_AS(Range(TestType(3), TestType(2)), std::runtime_error);
    CHECK_THROWS_AS(Range(TestType(2), TestType(2)), std::runtime_error);
}

TEMPLATE_TEST_CASE("Range constructor", "[range]", int, double)
{
    constexpr TestType lo{1};
    constexpr TestType mid{2};
    constexpr TestType hi{3};

    const Range r{lo, hi};
    CHECK(r.low() == lo);
    CHECK(r.mid() == mid);
    CHECK(r.high() == hi);
    CHECK(r.range() == hi - lo);

    CHECK_FALSE(r.includes(0));
    CHECK(r.includes(1));
    CHECK(r.includes(2));
    CHECK(r.includes(3));
    CHECK_FALSE(r.includes(4));
}

TEMPLATE_TEST_CASE("Using ranges", "[range]", int, double)
{
    constexpr TestType tlo{0};
    constexpr TestType low{1};
    constexpr TestType mid{2};
    constexpr TestType hi{3};
    constexpr TestType thi{4};

    const Range r{low, hi};
    std::array<TestType, 5> inputs = {{tlo, low, mid, hi, thi}};
    std::array<TestType, 5> clamped = {{low, low, mid, hi, hi}};
    std::array<TestType, 5> modulo = {{mid, low, mid, low, mid}};

    for (size_t i = 0; i < inputs.size(); ++i)
    {
        INFO(inputs[i]);
        TestType c{inputs[i]};
        r.clamp(c);
        CHECK(c == clamped[i]);
        TestType m{inputs[i]};
        r.modulo(m);
        CHECK(m == modulo[i]);
    }
}

TEST_CASE("Modulo", "[range]")
{
    const auto r = Range<double>::radians();
    CHECK(r.modulo(0.) == 0.);
    CHECK(r.modulo(1.) == 1.);
    CHECK(r.modulo(r.range()) == 0.);
    CHECK(r.modulo(-42.) == -42. + 7. * r.range());
    CHECK(r.modulo(-2.6) == -2.6 + r.range());
    CHECK(r.modulo(8.) == 8. - r.range());
    CHECK(r.modulo(99.) == 99. - 15. * r.range());

    constexpr double lo{3.1};
    constexpr double mi{4.};
    constexpr double hi{6.2};
    const Range<double> rx{lo, hi};
    CHECK(rx.modulo(lo) == lo);
    CHECK(rx.modulo(mi) == mi);
    CHECK(rx.modulo(hi) == lo);
    CHECK(rx.modulo(-42.) == -42. + 15. * rx.range());
    CHECK(rx.modulo(-2.6) == -2.6 + 2. * rx.range());
    CHECK(rx.modulo(8.) == 8. - rx.range());
    CHECK(rx.modulo(99.) == 99. - 30. * rx.range());
}
