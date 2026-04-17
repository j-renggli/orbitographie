#pragma once

#include <cmath>
#include <vector>

namespace galaxias
{
namespace math
{
namespace rng
{

template <class T>
struct PoissonDistribution
{
    using value_type = T;

    PoissonDistribution()
        : mean_{0}
        , current_{0}
        , i_{0}
    {
    }

    PoissonDistribution(T mean)
        : mean_{mean}
        , current_{std::exp(-mean)}
        , i_{0}
    {
    }

    PoissonDistribution(const PoissonDistribution& rhs) = default;
    PoissonDistribution(PoissonDistribution&& rhs) noexcept = default;
    ~PoissonDistribution() = default;

    PoissonDistribution& operator=(const PoissonDistribution& rhs) = default;
    PoissonDistribution& operator=(PoissonDistribution&& rhs) noexcept = default;

    bool operator!=(const PoissonDistribution& rhs) const { return mean_ != rhs.mean_; }
    PoissonDistribution& operator++()
    {
        current_ *= mean_ / static_cast<T>(++i_);
        return *this;
    }
    PoissonDistribution operator++(int)
    {
        PoissonDistribution copy{*this};
        ++(*this);
        return copy;
    }
    T operator*() { return current_; }

    static std::vector<T> generate(T mean, size_t count)
    {
        PoissonDistribution pd(mean);
        std::vector<T> values(count);
        for (size_t i = 0; i < count; ++i)
        {
            values[i] = *pd;
            ++pd;
        }

        return values;
    }

private:
    T mean_;
    T current_;
    T i_;
};

} // namespace rng
} // namespace math
} // namespace galaxias
