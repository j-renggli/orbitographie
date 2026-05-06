#include <math/rng/prng.h>

#include <math/range.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <deque>

using namespace galaxias;
using namespace math;
using namespace rng;

namespace
{
constexpr Range<double> range(10., 20.);
constexpr Range<size_t> intRange(15, 30);

} // namespace

TEST_CASE("Mersenne with arbitrary seed")
{
    Mersenne m{};

    const double x = m.uniform(range);
    CHECK(x >= 10.);
    CHECK(x <= 20.);
}

TEST_CASE("Mersenne with seed")
{
    Mersenne m{42};

    // m produces deterministic outputs
    CHECK(m.uniform(range) == Catch::Approx(17.5515553295));
    CHECK(m.uniform() == 11788048577503494824ull);
    CHECK(m.gaussian<double>() == Catch::Approx(-0.5740948067));
    CHECK(m.gaussian(10.) == Catch::Approx(8.0933146552));
    CHECK(m.gaussian(-5., 0.3) == Catch::Approx(-5.2172368796));
    CHECK(m.uniform(intRange) == 15);
    CHECK(m.gamma(1., 1.) == Catch::Approx(3.91177242184738549));
    CHECK(m.gamma(4.75, 0.2) == Catch::Approx(0.75500807216949284));
}

TEST_CASE("Gamma function is always positive")
{
    Mersenne m{42};
    for (size_t i = 0; i < 100; ++i)
    {
        REQUIRE(m.gamma(1., 1.) > 0.);
    }
}

TEST_CASE("Mersenne from existing and mask")
{
    constexpr int64_t mask = 0x7777777777777777;
    Mersenne m{42};
    Mersenne n{m, mask};

    // m has been updated in the process
    CHECK(m.uniform() == 11788048577503494824ull);
    // n produces deterministic outputs
    CHECK(n.uniform() == 15816580110690532602ull);
}
