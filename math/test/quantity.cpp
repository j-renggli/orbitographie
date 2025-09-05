#include <math/quantity.h>

#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace math;
using namespace quantity;

namespace
{
constexpr double two{2.};
constexpr double pi{3.14};

template <class R>
void checkRatio(double expected)
{
    CHECK(static_cast<double>(R::num) / static_cast<double>(R::den) == expected);
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
    {
        INFO("Unitless");
        const Unitless x{pi};
        CHECK(x.value() == pi);
        checkUnit(x, {{0., 0., 0., 0., 0., 0., 0., 0.}});
    }
    {
        INFO("rad");
        const Radian rad{pi};
        CHECK(rad.value() == pi);
        checkUnit(rad, {{0., 0., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Time")
{
    {
        INFO("s");
        const Second s{pi};
        CHECK(s.value() == pi);
        checkUnit(s, {{1., 0., 0., 0., 0., 0., 0., 0.}});
    }
    {
        INFO("s-1");
        const Frequency f{pi};
        CHECK(f.value() == pi);
        checkUnit(f, {{-1., 0., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Length")
{
    {
        INFO("m");
        const Metre m{pi};
        CHECK(m.value() == pi);
        checkUnit(m, {{0., 1., 0., 0., 0., 0., 0., 0.}});
    }
    {
        INFO("m-1");
        const PerMetre m{pi};
        CHECK(m.value() == pi);
        checkUnit(m, {{0., -1., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Mass")
{
    const Kilogram kg{pi};
    CHECK(kg.value() == pi);
    checkUnit(kg, {{0., 0., 1., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Temperature")
{
    const Kelvin k{pi};
    CHECK(k.value() == pi);
    checkUnit(k, {{0., 0., 0., 1., 0., 0., 0., 0.}});
}

TEST_CASE("Composite")
{
    {
        INFO("m/s");
        const Velocity v{two};
        CHECK(v.value() == two);
        checkUnit(v, {{-1., 1., 0., 0., 0., 0., 0., 0.}});
    }
    {
        INFO("W");
        const Watt w{two};
        CHECK(w.value() == two);
        checkUnit(w, {{-3., 2., 1., 0., 0., 0., 0., 0.}});
    }
    {
        INFO("m*m*m/s*s");
        auto mu = Metre{2.} * Metre{3.} * Metre{4.} / (Second{6.} * Second{2.});
        CHECK(mu.value() == 2.);
        checkUnit(mu, {{-2., 3., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Unary minus")
{
    const Metre mi{pi};
    const Metre mo = -mi;

    CHECK(mo.value() == -pi);
    checkUnit(mo, {{0., 1., 0., 0., 0., 0., 0., 0.}});
}

TEST_CASE("Multiplication")
{
    {
        // Same dimension
        INFO("m*m");
        const Metre ma{two};
        const Metre mb{pi};
        const auto m2 = ma * mb;

        CHECK(m2.value() == two * pi);
        checkUnit(m2, {{0., 2., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Different dimensions
        INFO("m*s");
        const Metre m{two};
        const Second s{pi};
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
        INFO("m/m");
        const Metre ma{pi};
        const Metre mb{two};
        const auto m2 = ma / mb;

        CHECK(m2.value() == pi / two);
        checkUnit(m2, {{0., 0., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Different dimensions
        const Metre m{two};
        const Second s{pi};
        const auto ms = m / s;
        const auto sm = s / m;

        {
            INFO("m/s");
            CHECK(ms.value() == two / pi);
            checkUnit(ms, {{-1., 1., 0., 0., 0., 0., 0., 0.}});
        }
        {
            INFO("s/m");
            CHECK(sm.value() == pi / two);
            checkUnit(sm, {{1., -1., 0., 0., 0., 0., 0., 0.}});
        }
    }
}

TEST_CASE("Power")
{
    {
        // Square
        INFO("m*m");
        const Metre ma{two};
        const auto m2 = ma.pow<2>();

        CHECK(m2.value() == two * two);
        checkUnit(m2, {{0., 2., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Inverse
        const Velocity v{two};
        INFO("m/s -1");
        const auto x = v.pow<-1>();

        CHECK(x.value() == 1. / two);
        checkUnit(x, {{1., -1., 0., 0., 0., 0., 0., 0.}});
    }
}

TEST_CASE("Root")
{
    {
        // Square root
        INFO("m*m");
        const Metre ma{two};
        const auto m2 = ma * ma;
        const auto m = m2.root<2>();

        CHECK(m.value() == two);
        checkUnit(m, {{0., 1., 0., 0., 0., 0., 0., 0.}});
    }

    {
        // Inverse
        const Velocity v{two};
        INFO("m/s -1");
        const auto x = v.root<-1>();

        CHECK(x.value() == 1. / two);
        checkUnit(x, {{1., -1., 0., 0., 0., 0., 0., 0.}});
    }
}
