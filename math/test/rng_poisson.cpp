#include <math/rng/poisson.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace galaxias;
using namespace math;
using namespace rng;

namespace
{

constexpr size_t cutoff{5};
constexpr size_t longCutoff{8};

} // namespace

TEST_CASE("Poisson for 0")
{
    const float zero = 0.f;
    const auto poisson = generatePoisson(zero, cutoff);
    REQUIRE(poisson.size() == cutoff);
    CHECK(poisson[0] == 1.f);
    for (size_t i = 1; i < cutoff; ++i)
    {
        CHECK(poisson[i] == zero);
    }
}

TEST_CASE("Poisson for 1")
{
    const double one = 1.;
    const double target = 0.367879;
    const auto poisson = generatePoisson(one, cutoff);
    REQUIRE(poisson.size() == cutoff);
    CHECK_THAT(poisson[0], Catch::Matchers::WithinRel(target, 1e-5));
    CHECK_THAT(poisson[1], Catch::Matchers::WithinRel(target, 1e-5));
    CHECK_THAT(poisson[2], Catch::Matchers::WithinRel(target / 2., 1e-5));
    CHECK_THAT(poisson[3], Catch::Matchers::WithinRel(target / 6., 1e-5));
    CHECK_THAT(poisson[4], Catch::Matchers::WithinRel(target / 24., 1e-5));
}

TEST_CASE("Poisson for 2")
{
    const double two = 2.;
    const double target = 0.27067;
    const auto poisson = generatePoisson(two, longCutoff);
    REQUIRE(poisson.size() == longCutoff);
    CHECK_THAT(poisson[0], Catch::Matchers::WithinRel(target / 2, 1e-5));
    CHECK_THAT(poisson[1], Catch::Matchers::WithinRel(target, 1e-5));
    CHECK_THAT(poisson[2], Catch::Matchers::WithinRel(target, 1e-5));
    CHECK_THAT(poisson[3], Catch::Matchers::WithinRel(target / 1.5, 1e-5));
    CHECK_THAT(poisson[4], Catch::Matchers::WithinRel(target / 3., 1e-5));
    CHECK_THAT(poisson[5], Catch::Matchers::WithinRel(target / 7.5, 1e-5));
    CHECK_THAT(poisson[6], Catch::Matchers::WithinRel(target / 22.5, 1e-5));
    CHECK_THAT(poisson[7], Catch::Matchers::WithinRel(target / 78.75, 1e-5));
}

TEST_CASE("Poisson for 2 and half")
{
    const double twoHalf = 2.5;
    const double target = 0.2565156;
    const auto poisson = generatePoisson(twoHalf, longCutoff);
    REQUIRE(poisson.size() == longCutoff);
    CHECK_THAT(poisson[0], Catch::Matchers::WithinRel(target / 3.125, 1e-5));
    CHECK_THAT(poisson[1], Catch::Matchers::WithinRel(target / 1.25, 1e-5));
    CHECK_THAT(poisson[2], Catch::Matchers::WithinRel(target, 1e-5));
    CHECK_THAT(poisson[3], Catch::Matchers::WithinRel(target / 1.2, 1e-5));
    CHECK_THAT(poisson[4], Catch::Matchers::WithinRel(target / 1.92, 1e-5));
    CHECK_THAT(poisson[5], Catch::Matchers::WithinRel(target / 3.84, 1e-5));
    CHECK_THAT(poisson[6], Catch::Matchers::WithinRel(target / 9.216, 1e-5));
    CHECK_THAT(poisson[7], Catch::Matchers::WithinRel(target / 25.8048, 1e-5));
}
