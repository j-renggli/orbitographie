#include <orbit/position.h>

#include <math/rng/prng.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace galaxias;
using namespace orbit;
using namespace coordinates;

TEST_CASE("Cartesian")
{
    const Cartesian coords{Eigen::Vector3d{1., 2., 3.}, Eigen::Vector3d{4., 5., 6.}};
    CHECK(coords.position()[0] == 1.);
    CHECK(coords.position()[1] == 2.);
    CHECK(coords.position()[2] == 3.);
    CHECK(coords.velocity()[0] == 4.);
    CHECK(coords.velocity()[1] == 5.);
    CHECK(coords.velocity()[2] == 6.);
}

TEST_CASE("Cylindrical")
{
    auto check1 = [](const position::Cylindrical& pos, double r, double a, double h)
    {
        CHECK(pos.radius.value() == r);
        CHECK(pos.angle.value() == a);
        CHECK(pos.height.value() == h);
    };

    auto check2 = [](const position::Cartesian& pos, double x, double y, double z)
    {
        CHECK_THAT(pos.value().x(), Catch::Matchers::WithinAbs(x, 1e-10));
        CHECK_THAT(pos.value().y(), Catch::Matchers::WithinAbs(y, 1e-10));
        CHECK_THAT(pos.value().z(), Catch::Matchers::WithinAbs(z, 1e-10));
    };

    auto check3 = [](const position::Cylindrical& pos)
    {
        const auto cartesian = pos.toCartesian();
        const position::Cylindrical back{cartesian};
        CHECK(pos.radius.value() == Catch::Approx(back.radius.value()));
        CHECK(pos.angle.value() == Catch::Approx(back.angle.value()));
        CHECK(pos.height.value() == Catch::Approx(back.height.value()));
    };

    {
        INFO("1 0 2");
        const position::Cylindrical pos{1., 0., 2.};
        check1(pos, 1., 0., 2.);
        check2(pos.toCartesian(), 1., 0., 2.);
        check3(pos);
    }

    {
        INFO("5 270 0");
        const position::Cylindrical pos{5., M_PI * 3. / 2., 0.};
        check1(pos, 5., M_PI * 3. / 2., 0.);
        check2(pos.toCartesian(), 0., -5., 0.);
        check3(pos);
    }

    {
        const uint64_t seed = std::random_device{}();
        galaxias::math::rng::Mersenne rng{seed};
        const auto circle = qty::angle::FullRadian::range();
        const double r = rng.uniform(circle);
        const double a = rng.uniform(circle);
        const double h = rng.uniform(circle);
        INFO(r << " " << a << " " << h);
        const position::Cylindrical pos{r, a, h};
        check1(pos, r, a, h);
        check2(pos.toCartesian(), r * cos(a), r * sin(a), h);
        check3(pos);
    }
}
