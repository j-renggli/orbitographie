#pragma once

#include <cmath>
#include <stdexcept>

namespace galaxias
{
namespace math
{

template <class T>
class Range
{
public:
    using value_type = T;

    constexpr Range(const T& low, const T& high)
        : lo_{low}
        , hi_{high}
    {
        if (low >= high)
        {
            throw std::runtime_error("Range must have low < high; has " + std::to_string(low) +
                                     " >= " + std::to_string(high));
        }
    }
    Range(const Range&) = default;
    Range(Range&&) noexcept = default;
    ~Range() = default;

    Range& operator=(const Range&) = default;
    Range& operator=(Range&&) noexcept = default;

    bool operator==(const Range& rhs) const { return lo_ == rhs.lo_ && hi_ == rhs.hi_; }
    bool operator!=(const Range& rhs) const { return !operator==(rhs); }

    const T& low() const noexcept { return lo_; }
    const T& high() const noexcept { return hi_; }
    T mid() const noexcept { return (lo_ + hi_) * 0.5; }
    const T range() const noexcept { return hi_ - lo_; }

    bool includes(const T& value) const;

    void clamp(T& value) const;
    T clamp(const T& value) const;
    void modulo(T& value) const;
    T modulo(const T& value) const;

    static Range make(const T& a, const T& b) { return Range{std::min(a, b), std::max(a, b)}; }

    static constexpr Range<T> positive() { return Range<T>{0., std::numeric_limits<double>::max()}; }
    static constexpr Range<T> negative() { return Range<T>{-std::numeric_limits<double>::max(), 0.}; }
    static constexpr Range<T> zeroOne() { return Range<T>{0., 1.}; }
    static constexpr Range<T> radians() { return Range<T>{0., 2. * M_PI}; }

private:
    T lo_;
    T hi_;
};

} // namespace math
} // namespace galaxias

#include "range.inl"
