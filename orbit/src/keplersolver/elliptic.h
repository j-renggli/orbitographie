#pragma once

#include "solver.h"

namespace galaxias
{
namespace orbit
{

class EllipticKeplerSolver : public UniversalKeplerSolver
{
public:
    EllipticKeplerSolver(CenterOfMass&& com)
        : UniversalKeplerSolver{std::move(com)}
        , period_{com.orbitalPeriod()}
    {
    }

    void setTargetTime(const qty::Second& targetTime) override
    {
        // Simplify solution by taking only one period into account
        h_ = period_.modulo(targetTime.value() - t0_);
    }

    double f(double s) const override
    {
        const double s2 = sin(sb_ * s * 0.5);
        const double c2 = cos(sb_ * s * 0.5);

        return (2. * s2 * (c2 * (sb_ * r0_ - k_ / sb_) + rdotv_ * s2) + k_ * s) / beta_ - h_;
    }

    double df(double s) const override
    {
        const double s2 = sin(sb_ * s * 0.5);
        const double c2 = cos(sb_ * s * 0.5);

        return df(s2, c2);
    }

    Factors factorsAt(double s) const override
    {
        const double s2 = sin(sb_ * s * 0.5);
        const double c2 = cos(sb_ * s * 0.5);
        const double r = df(s2, c2);

        const double twoS2 = 2. * s2;
        const double num = twoS2 * k_ * s2;

        return {1. - num / (r0_ * beta_),                           // f
                twoS2 * ((r0_ * c2 / sb_) + (rdotv_ * s2 / beta_)), // g
                -twoS2 * k_ * c2 / (r0_ * r * sb_),                 // f'
                1. - num / (r * beta_)};                            // g'
    }

private:
    double df(double s2, double c2) const
    {
        return r0_ + 2. * rdotv_ * s2 * c2 / sb_ + 2. * s2 * s2 * (k_ / beta_ - r0_);
    }

    math::Range<double> bisectionRange() const override
    {
        const double invPeriod = (sb_ * beta_) / (2. * M_PI * k_);
        const double sPerOrbit = 2. * M_PI / sb_;
        const double sMin = sPerOrbit * std::floor(h_ * invPeriod);
        return math::Range<double>(sMin, sMin + sPerOrbit);
    }

    math::Range<double> period_;
};

} // namespace orbit
} // namespace galaxias
