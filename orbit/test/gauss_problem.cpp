#include <orbit/centerofmass.h>

#include <math/solver/function.h>
#include <math/solver/newton_raphson.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace galaxias;
using namespace orbit;
using namespace coordinates;

namespace ratio = math::unit::ratio;

class GaussProblem : public math::solver::IFunction
{
protected:
    using MeterSquared = qty::Quantity<double, math::unit::MetreSquared>;

    /// Constants
    static constexpr qty::Unitless one{1.};
    const GravitationalParam mu_;
    const coordinates::Cartesian::Position pos1_;
    const coordinates::Cartesian::Position pos2_;
    const qty::Metre r1_;
    const qty::Metre r2_;
    const MeterSquared r1r2_;
    const qty::Unitless cosdv_;
    const qty::Radian dv_;
    const MeterSquared pgNum_;
    const qty::Unitless tandv2_;
    const MeterSquared k_;
    const qty::Quantity<double, math::unit::Unit<ratio::Zero, std::ratio<4>>> k2_;
    const qty::Metre l_;
    const MeterSquared l2_;
    const MeterSquared m_;
    const qty::Metre p1_;
    const qty::Metre p2_;

    /// Variables
    qty::Second target_{std::numeric_limits<double>::quiet_NaN()};
    qty::Metre p_{0.};
    qty::PerMetre alpha_{0.};
    qty::Unitless f_{0.};
    qty::Second g_{0.};
    qty::Frequency df_{0.};
    qty::Second t_{std::numeric_limits<double>::quiet_NaN()};
    qty::Quantity<double, math::unit::DivideUnit<math::unit::Second, math::unit::Metre>::value_type> dtdp_{
        std::numeric_limits<double>::quiet_NaN()};

public:
    GaussProblem(const GravitationalParam& mu,
                 const coordinates::Cartesian::Position& pos1,
                 const coordinates::Cartesian::Position& pos2,
                 bool longWay = false)
        : mu_{mu}
        , pos1_{pos1}
        , pos2_{pos2}
        , r1_{pos1_.norm()}
        , r2_{pos2_.norm()}
        , r1r2_{r1_ * r2_}
        , cosdv_{pos1_.dot(pos2_) / r1r2_}
        , dv_{longWay ? (2. * M_PI - acos(cosdv_.value())) : acos(cosdv_.value())}
        , pgNum_{r1r2_ * sin(dv_.value())}
        , tandv2_{tan(dv_.value() * 0.5)}
        , k_{r1r2_ * (-cosdv_ + 1.)}
        , k2_{k_ * k_}
        , l_{r1_ + r2_}
        , l2_{l_ * l_}
        , m_{r1r2_ * (cosdv_ + 1.)}
        , p1_{k_ / (l_ + (m_ * 2.).root<2>())}
        , p2_{k_ / (l_ - (m_ * 2.).root<2>())}
    {
        if (pos1.cross(pos2).squaredNorm() == 0.)
        {
            // TODO: Find a solution (?)
            throw std::runtime_error("Can't find a solution for the gauss problem if the positions are collinear");
        }
    }

    double minP() const { return p1_.value(); }
    double limitP() const { return p2_.value(); }
    double initialGuess() const { return (p1_ + p2_).value() * 0.5; }

    void setTargetTime(const qty::Second& targetTime) { target_ = targetTime; }

    /// Pre-compute most variables for a given p
    void setP(double x)
    {
        if (p_.value() == x)
        {
            // Already set
            return;
        }

        p_ = x;
        const MeterSquared p2 = p_ * p_;
        f_ = (cosdv_ - one) * r2_ / p_ + one;
        g_ = pgNum_ / (mu_ * p_).root<2>();
        df_ = -(mu_ / p_).root<2>() * tandv2_ * ((cosdv_ - one) / p_ + one / r1_ + one / r2_);

        const qty::PerMetre alpha = ((m_ * 2. - l2_) * p2 + k_ * l_ * p_ * 2. - k2_) / (m_ * k_ * p_);
        const qty::Unitless cDX = one - alpha * r1_ * (one - f_); // Either cosDE or coshDE

        alpha_ = alpha;

        if (alpha >= 0.)
        {
            // Elliptic, parabolic too?
            const qty::Unitless sinDE = -r1r2_ * df_ / (mu_ / alpha).root<2>();
            const qty::Radian dE = (sinDE < 0. ? 2 * M_PI : 0.) + atan2(sinDE.value(), cDX.value());
            t_ = g_ + (dE - sinDE) / (mu_ * alpha.pow<3>()).root<2>();
            dtdp_ = -g_ / (p_ * 2.) - (t_ - g_) * 1.5 * (k2_ + (m_ * 2. - l2_) * p2) / (alpha * m_ * k_ * p2) +
                    k_ * 2. * sinDE / (p_ * (k_ - l_ * p_) * (mu_ * alpha.pow<3>()).root<2>());
        }
        else if (alpha < 0.)
        {
            // Hyperbolic
            const qty::Radian dF = acosh(cDX.value());
            const qty::Unitless sinhDeltaF{sinh(dF.value())};
            t_ = g_ + (sinhDeltaF - dF) / (-mu_ * alpha.pow<3>()).root<2>();
            dtdp_ = -g_ / (p_ * 2.) - (t_ - g_) * 1.5 * (k2_ + (m_ * 2. - l2_) * p2) / (alpha * m_ * k_ * p2) -
                    k_ * 2. * sinhDeltaF / (p_ * (k_ - l_ * p_) * (-mu_ * alpha.pow<3>()).root<2>());
        }

        // Cheat to ensure Newton-raphson never reaches p <= 0.
        if (x - ((t_ - target_) / dtdp_).value() <= 0.)
        {
            dtdp_ = (t_ - target_) * 2. / p_;
        }
    }

    std::pair<coordinates::Cartesian::Velocity, coordinates::Cartesian::Velocity> velocitiesAt(double p)
    {
        setP(p);

        const qty::Unitless dg = one - r1_ * (one - cosdv_) / p_;
        const coordinates::Cartesian::Velocity v1 = (pos2_ - pos1_ * f_) / g_;
        return std::make_pair(v1, pos1_ * df_ + v1 * dg);
    }

    double f(double x) const override
    {
        const_cast<GaussProblem&>(*this).setP(x);

        return (t_ - target_).value();
    }

    double df(double x) const override
    {
        const_cast<GaussProblem&>(*this).setP(x);

        return dtdp_.value();
    }
};

struct Expectation
{
    double r1;
    double r2;
    double dv;
    double k;
    double l;
    double m;
    double p1;
    double p2;
};

class GaussProblemTest : public GaussProblem
{
public:
    GaussProblemTest(const GravitationalParam& mu,
                     const coordinates::Cartesian::Position& pos1,
                     const coordinates::Cartesian::Position& pos2,
                     const Expectation& expected,
                     bool longWay = false)
        : GaussProblem{mu, pos1, pos2, longWay}
    {
        CHECK(r1_.value() == Catch::Approx(expected.r1));
        CHECK(r2_.value() == Catch::Approx(expected.r2));
        CHECK(dv_.value() == Catch::Approx(expected.dv));
        CHECK(k_.value() == Catch::Approx(expected.k));
        CHECK(l_.value() == Catch::Approx(expected.l));
        CHECK(m_.value() == Catch::Approx(expected.m));
        CHECK(p1_.value() == Catch::Approx(expected.p1));
        CHECK(p2_.value() == Catch::Approx(expected.p2));
    }

    double alpha() const { return alpha_.value(); }
};

TEST_CASE("Gauss problem 1")
{
    const GravitationalParam mu{3.964016e-14};
    const double t{207. * 24. * 3600.};
    coordinates::Cartesian::Position r1{{0.473265, -0.899215, 0.}};
    coordinates::Cartesian::Position r2{{0.066842, 1.561256, 0.030948}};
    Expectation expected{1.016153, 1.562993, 2.6139965, 2.960511, 2.579146, 0.215969, 0.914764, 1.540388};
    GaussProblemTest problem{mu, r1, r2, expected};
    problem.setTargetTime(t);
    CHECK(problem.f(1.2) + t == Catch::Approx(21380951.));
    CHECK(problem.df(1.2) == Catch::Approx(-83159196.3342416734));

    double p = math::solver::NewtonRaphson::findRoot(problem, 1.2);
    CHECK(p == Catch::Approx(1.250633));
    const auto vs = problem.velocitiesAt(p);
    CHECK(1e6 * vs.first[0].value() == Catch::Approx(0.193828));
    CHECK(1e6 * vs.first[1].value() == Catch::Approx(0.101824));
    CHECK(1e6 * vs.first[2].value() == Catch::Approx(0.00861759));
    CHECK(1e6 * vs.second[0].value() == Catch::Approx(-0.141359));
    CHECK(1e6 * vs.second[1].value() == Catch::Approx(0.02670098));
    CHECK(1e6 * vs.second[2].value() == Catch::Approx(-0.00443406));
}

TEST_CASE("Gauss problem 2")
{
    const GravitationalParam mu{1.};
    const double t{1.092};
    coordinates::Cartesian::Position r1{{1., 0., 0.}};
    coordinates::Cartesian::Position r2{{1., 1., 1.}};
    Expectation expected{
        1., sqrt(3.), 0.9553166181, 0.7320508076, 2.7320508076, 2.7320508076, 0.1444003228, 1.8555996772};
    GaussProblemTest problem{mu, r1, r2, expected};
    problem.setTargetTime(t);
    CHECK(problem.f(2.) + t == Catch::Approx(1.1036324682));
    CHECK(problem.df(2.) == Catch::Approx(-0.3325605584));

    const double p = math::solver::NewtonRaphson::findRoot(problem, 2.);
    CHECK(p == Catch::Approx(2.035532939));
    const auto vs = problem.velocitiesAt(p);
    const auto h1 = r1.cross(vs.first);
    const auto h2 = r2.cross(vs.second);
    CHECK(h1[0].value() == 0.);
    CHECK(h1[1].value() == Catch::Approx(-1.00884));
    CHECK(h1[2].value() == Catch::Approx(1.00884));
    CHECK(h1.squaredNorm() == h2.squaredNorm());
}

TEST_CASE("Gauss problem 2 bis")
{
    // Same problem as above, different units
    const GravitationalParam mu{3.986004418e14};
    const double t{1.092 * 806.8118744};
    constexpr double radius{6378136.};
    constexpr double timeFactor{806.8118744};
    coordinates::Cartesian::Position r1{{radius, 0., 0.}};
    coordinates::Cartesian::Position r2{{radius, radius, radius}};
    Expectation expected{radius,
                         radius * sqrt(3.),
                         0.9553166181,
                         29780279870194.46875,
                         17425391.6095841303,
                         111141517539186.46875,
                         921004.8974705327,
                         11835267.1025294587};
    GaussProblemTest problem{mu, r1, r2, expected};
    problem.setTargetTime(t);
    CHECK(problem.f(2. * radius) + t == Catch::Approx(1.1036324682 * timeFactor));
    CHECK(problem.df(2. * radius) == Catch::Approx(-0.0000420677));

    const double p = math::solver::NewtonRaphson::findRoot(problem, 2. * radius);
    CHECK(p == Catch::Approx(2.035532939 * radius));
    const auto vs = problem.velocitiesAt(p);
    const auto h1 = r1.cross(vs.first);
    const auto h2 = r2.cross(vs.second);
    CHECK(h1[0].value() == 0.);
    CHECK(h1[1].value() == Catch::Approx(-1.00884 * radius * radius / timeFactor));
    CHECK(h1[2].value() == Catch::Approx(1.00884 * radius * radius / timeFactor));
    CHECK(h1.squaredNorm() == h2.squaredNorm());
}

TEST_CASE("Gauss problem 3")
{
    const GravitationalParam mu{1.};
    const double t{1. / 8.};
    coordinates::Cartesian::Position r1{{1., 0., 0.}};
    coordinates::Cartesian::Position r2{{1., 1. / 8., 1. / 8.}};
    Expectation expected{
        1., 1.0155048006, 0.1749690457, 0.0155048006, 2.0155048006, 2.0155048006, 0.0038538074, 1.9961461926};
    GaussProblemTest problem{mu, r1, r2, expected};
    problem.setTargetTime(t);
    CHECK(problem.f(2.) + t == Catch::Approx(0.1253217704));
    CHECK(problem.df(2.) == Catch::Approx(-0.0314917005));

    const double p = math::solver::NewtonRaphson::findRoot(problem, 2.);
    CHECK(p == Catch::Approx(2.0102571115));
    const auto vs = problem.velocitiesAt(p);
    CHECK(vs.first[0].value() == Catch::Approx(0.061861));
    CHECK(vs.first[1].value() == Catch::Approx(1.00256));
    CHECK(vs.first[2].value() == Catch::Approx(1.00256));
}

TEST_CASE("Gauss problem 4")
{
    const GravitationalParam mu{1.};
    const double t{20.};
    coordinates::Cartesian::Position r1{{0.5, 0.6, 0.7}};
    coordinates::Cartesian::Position r2{{0., -1., 0.}};
    GaussProblem problem{mu, r1, r2, true};
    problem.setTargetTime(t);
    CHECK(problem.initialGuess() == Catch::Approx(1.0236648961));
    CHECK(problem.f(problem.initialGuess()) + t == Catch::Approx(4.2541680127));

    const double p = math::solver::NewtonRaphson::findRoot(problem, problem.initialGuess());
    CHECK(p == Catch::Approx(1.3282281153));
    const auto vs = problem.velocitiesAt(p);
    CHECK(vs.second[0].value() == Catch::Approx(0.66986992));
    CHECK(vs.second[1].value() == Catch::Approx(0.48048471));
    CHECK(vs.second[2].value() == Catch::Approx(0.93781789));
}
