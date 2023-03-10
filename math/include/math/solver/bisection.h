#pragma once

#include "solver.h"

namespace galaxias
{
namespace math
{
namespace solver
{

class Bisection : public ISolver
{
public:
    virtual ~Bisection() = default;

    double rootOf(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7) const override;

    static double findRoot(const IFunction& fct, const Range<double>& range, const double tolerance = 1e-7);
};

} // namespace solver
} // namespace math
} // namespace galaxias
