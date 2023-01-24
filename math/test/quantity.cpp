#include <math/quantity.h>

#include <catch2/catch.hpp>

namespace
{
constexpr double two{2.};
constexpr double pi{3.14};

template <class R> void checkRatio(double expected)
{
    CHECK(static_cast<double>(R::num) / static_cast<double>(R::den) ==
          expected);
}

template <class T>
void checkUnit(const T&, const std::array<double, 8>& expectedRatios)
{
    {
        INFO("Time");
        checkRatio<typename T::Unit::Time>(expectedRatios[0]);
    }
    {
        INFO("Length");
        checkRatio<typename T::Unit::Length>(expectedRatios[1]);
    }
}

} // namespace

TEST_CASE("Unitless")
{
    const math::Unitless x{pi};
    CHECK(x.value() == pi);
    checkUnit(x, {{0., 0., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Second")
{
    const math::Second s{pi};
    CHECK(s.value() == pi);
    checkUnit(s, {{1., 0., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Metre")
{
    const math::Metre m{pi};
    CHECK(m.value() == pi);
    checkUnit(m, {{0., 1., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Multiplication")
{
    {
        // Same dimension
        const math::Metre ma{two};
        const math::Metre mb{pi};
        const auto m2 = ma * mb;

        CHECK(m2.value() == two * pi);
        checkUnit(m2, {{0., 2., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Different dimensions
        const math::Metre m{two};
        const math::Second s{pi};
        const auto ms = m * s;
        const auto sm = s * m;

        CHECK(ms.value() == two * pi);
        checkUnit(ms, {{1., 1., 0., 0., 0., 0., 0., 0.}});
        CHECK(sm.value() == two * pi);
        checkUnit(sm, {{1., 1., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Division")
{
    {
        // Same dimension
        const math::Metre ma{pi};
        const math::Metre mb{two};
        const auto m2 = ma / mb;

        CHECK(m2.value() == pi / two);
        checkUnit(m2, {{0., 0., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Different dimensions
        const math::Metre m{two};
        const math::Second s{pi};
        const auto ms = m / s;
        const auto sm = s / m;

        CHECK(ms.value() == two / pi);
        checkUnit(ms, {{-1., 1., 0., 0., 0., 0., 0., 0.}});
        CHECK(sm.value() == pi / two);
        checkUnit(sm, {{1., -1., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("m3_s2")
{
    auto mu = math::Metre{2.} * math::Metre{3.} * math::Metre{4.} /
              (math::Second{6.} * math::Second{2.});
    CHECK(mu.value() == 2.);
    checkUnit(mu, {{-2., 3., 0., 0., 0., 0., 0., 0.}});
}