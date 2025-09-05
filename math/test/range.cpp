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
