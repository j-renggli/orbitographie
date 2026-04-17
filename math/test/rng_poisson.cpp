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
    const auto poisson = PoissonDistribution<float>::generate(zero, cutoff);
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
    PoissonDistribution<float> pd(one);
    CHECK_THAT(*pd, Catch::Matchers::WithinRel(target, 1e-5));
    double fact = 1.;
    for (size_t i = 1; i < 20; ++i)
    {
        ++pd;
        fact *= static_cast<double>(i);
        CHECK_THAT((*pd), Catch::Matchers::WithinRel(target / fact, 1e-5));
    }
}

TEST_CASE("Poisson for 2")
{
    const double two = 2.;
    const double target = 0.27067;
    const auto poisson = PoissonDistribution<float>::generate(two, longCutoff);
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
    const float twoHalf = 2.5;
    const float target = 0.2565156;
    const auto poisson = PoissonDistribution<float>::generate(twoHalf, longCutoff);
    REQUIRE(poisson.size() == longCutoff);
    CHECK_THAT(poisson[0], Catch::Matchers::WithinRel(target / 3.125f, 1e-5f));
    CHECK_THAT(poisson[1], Catch::Matchers::WithinRel(target / 1.25f, 1e-5f));
    CHECK_THAT(poisson[2], Catch::Matchers::WithinRel(target, 1e-5f));
    CHECK_THAT(poisson[3], Catch::Matchers::WithinRel(target / 1.2f, 1e-5f));
    CHECK_THAT(poisson[4], Catch::Matchers::WithinRel(target / 1.92f, 1e-5f));
    CHECK_THAT(poisson[5], Catch::Matchers::WithinRel(target / 3.84f, 1e-5f));
    CHECK_THAT(poisson[6], Catch::Matchers::WithinRel(target / 9.216f, 1e-5f));
    CHECK_THAT(poisson[7], Catch::Matchers::WithinRel(target / 25.8048f, 1e-5f));
}
