#include <orbit/position.h>

#include <catch2/catch_test_macros.hpp>

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
