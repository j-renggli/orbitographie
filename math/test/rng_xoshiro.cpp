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

std::vector<float> quartiles{0.25, 0.25, 0.25, 0.25};

} // namespace

TEST_CASE("Xoshiro with arbitrary seed")
{
    Random m{};

    const double x = m.uniform(range);
    CHECK(x >= 10.);
    CHECK(x <= 20.);
}

TEST_CASE("Xoshiro with seed")
{
    Random m{42};

    // m produces deterministic outputs (check they match GPU ones)
    CHECK(m.uniform(range) == Catch::Approx(14.137017257));
    CHECK(m.uniform() == 9106390978755430941ull);
    CHECK(m.uniform() == 2879885515858120463ull);
    CHECK(m.uniform() == 4306470244779094640ull);
    CHECK(m.uniform() == 88310362294001210ull);
    CHECK(m.uniform() == 2309642221192634594ull);
    CHECK(m.uniform() == 12709356935175648966ull);
    CHECK(m.uniform() == 11932983601200282976ull);
    CHECK(m.uniform() == 15798042029967569691ull);
    CHECK(m.uniform() == 6836518967294217584ull);
    CHECK(m.uniform() == 1047958001686683753ull);
    CHECK(m.uniform() == 13531219975404544529ull);
    CHECK(m.uniform() == 4958810905591577550ull);
    CHECK(m.uniform() == 15696718992406987882ull);
    CHECK(m.uniform() == 16452031625413118338ull);
    CHECK(m.uniform() == 11256319044194010585ull);
    CHECK(m.uniform() == 14360080114247097181ull);
    CHECK(m.uniform() == 1551269651652525133ull);
    CHECK(m.uniform() == 11780291535123546661ull);
    CHECK(m.uniform() == 12311635331428359044ull);
    CHECK(m.gaussian<double>() == Catch::Approx(-0.8325193529));
    CHECK(m.gaussian(10.) == Catch::Approx(9.7031224002));
    CHECK(m.gaussian(-5., 0.3) == Catch::Approx(-5.1379749829));
    CHECK(m.uniform(intRange) == 22);
    CHECK(m.gamma(1., 1.) == Catch::Approx(2.11308086130961881));
    CHECK(m.gamma(4.75, 0.2) == Catch::Approx(1.56048225208235403));
}

TEST_CASE("Xoshiro from existing and mask")
{
    constexpr int64_t mask = 0x7777777777777777;
    Random m{42};
    Random n{m, mask};

    // m has been updated in the process
    CHECK(m.uniform() == 9106390978755430941ull);
    // n produces deterministic outputs
    CHECK(n.uniform() == 3349526164112356688ull);
}

// TEST_CASE("Realize function") {}

// TEST_CASE("Xoshiro index")
//{
////    Random m{42};

////    // Simple test with quartiles
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.) == 0);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.1) == 0);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.25) == 1);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.4) == 1);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.5) == 2);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.6) == 2);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.75) == 3);
////    CHECK(detail::realize(quartiles.begin(), quartiles.end(), 0.9) == 3);
////    CHECK_THROWS_AS(detail::realize(quartiles.begin(), quartiles.end(), 1.), std::out_of_range);
////    CHECK(m.realization(quartiles.begin(), quartiles.end()) == 3);

////    {
////        FixThenHalve fth({0.2, 0.3, 0.4});
////        CHECK(*fth == 0.2);
////        ++fth;
////        CHECK(*fth == 0.3);
////        ++fth;
////        CHECK(*fth == 0.4);
////        ++fth;
////        CHECK(*fth == Approx(0.05));
////        ++fth;
////        CHECK(*fth == Approx(0.025));
////        ++fth;
////        CHECK(*fth == Approx(0.0125));
////        ++fth;
////    }

////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.) == 0);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.1) == 0);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.2) == 1);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.4) == 1);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.5) == 2);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.7) == 2);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.9) == 3);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.95) == 4);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.975) == 5);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.9875) == 6);
////    CHECK(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 0.99999999) == 26);
////    CHECK_THROWS_AS(detail::realize(FixThenHalve({0.2, 0.3, 0.4}), FixThenHalve<double>(), 1.), std::out_of_range);
//}
