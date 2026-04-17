#pragma once

#include <math/solver/function.h>
#include <orbit/centerofmass.h>

namespace galaxias
{
namespace orbit
{

/// Universal kepler solver based on
/// Wisdom J, Hernandez DM.
/// A fast and accurate universal Kepler solver without Stumpff series.
/// \mnras. 2015;453 (3) :3015-3023.

class UniversalKeplerSolver : public math::solver::IFunction
{
public:
    UniversalKeplerSolver(const CenterOfMass& com);

    virtual ~UniversalKeplerSolver() {}

    /// Create an appropriate solver for the given initial conditions
    static std::unique_ptr<UniversalKeplerSolver> create(const CenterOfMass& com);

    /// Return the coordinates at time t
    coordinates::Cartesian coordinatesAt(const qty::Second& targetTime);

public:
    /// Get the value of s for the provided target time
    double solveForInternal(const qty::Second& targetTime);

    /// Once solved, we can retrieve useful values for validation
    double initialGuess() const { return guess_; }
    double computedS() const { return root_; }

    /// Get the factors at the given s
    struct Factors
    {
        double f;
        double g;
        double df;
        double dg;
    };
    virtual Factors factorsAt(double s) const = 0;

private:
    /// Set the target time and return the guess for initial value of s
    virtual void setTargetTime(const qty::Second& targetTime);

    /// Create range for bissection search
    virtual math::Range<double> bisectionRange() const = 0;

protected:
    const CenterOfMass& com_;
    double r0_;
    double rdotv_;
    double k_;
    double beta_;
    double sb_;
    double t0_;
    double h_;
    double guess_;
    double root_;
};

} // namespace orbit
} // namespace galaxias
