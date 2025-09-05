#pragma once

#include "../../src/keplersolver/solver.h"
#include <vector>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

namespace galaxias
{
namespace orbit
{
namespace test
{

struct Results
{
    std::vector<double> guess;
    std::vector<double> s;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> vx;
    std::vector<double> vy;

    void computeAndCheck(UniversalKeplerSolver& solver,
                         const std::vector<double>& t,
                         const Results& expected,
                         double h0,
                         const coordinates::Cartesian& initial = coordinates::Cartesian::zero())
    {
        for (const double& ti : t)
        {
            INFO(ti);
            const auto coords = solver.coordinatesAt(ti);
            s.push_back(solver.computedS());
            CHECK(std::abs(solver.f(s.back())) < 2e-3);
            guess.push_back(solver.initialGuess());
            x.push_back((coords.position() - initial.position())[0].value());
            y.push_back((coords.position() - initial.position())[1].value());
            CHECK(coords.position()[2].value() == 0.);
            vx.push_back((coords.velocity() - initial.velocity())[0].value());
            vy.push_back((coords.velocity() - initial.velocity())[1].value());
            CHECK(coords.velocity()[2].value() == 0.);

            // Ensure conservation of energy
            CHECK(coords.position().cross(coords.velocity())[2].value() == Catch::Approx(h0));
        }

        CHECK(s[6] * 1e25 == Catch::Approx(expected.s[6] * 1e25));
        CHECK(vx[6] * 1e25 == Catch::Approx(expected.vx[6] * 1e25));

        CHECK_THAT(guess, Catch::Matchers::Approx(expected.guess));
        CHECK_THAT(s, Catch::Matchers::Approx(expected.s));
        CHECK_THAT(x, Catch::Matchers::Approx(expected.x));
        CHECK_THAT(y, Catch::Matchers::Approx(expected.y));
        CHECK_THAT(vx, Catch::Matchers::Approx(expected.vx));
        CHECK_THAT(vy, Catch::Matchers::Approx(expected.vy));
    }
};

} // namespace test
} // namespace orbit
} // namespace galaxias
