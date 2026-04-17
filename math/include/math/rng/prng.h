#pragma once

#include "../range.h"

#include <array>
#include <random>

namespace galaxias
{
namespace math
{
namespace rng
{

namespace detail
{

template <class T>
inline size_t realise(T&& it, const T& end, typename T::value_type x)
{
    size_t i = 0;
    typename T::value_type sum{0};
    while (it != end)
    {
        sum += *it++;
        if (x < sum)
        {
            return i;
        }
        ++i;
    }

    throw std::out_of_range("Out of known distribution: " + std::to_string(x));
}

class Xoshiro256
{
public:
    using result_type = uint64_t;

    Xoshiro256(uint64_t seed);

    uint64_t operator()();

    static constexpr uint64_t min() { return 0; }
    static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }

private:
    std::array<uint64_t, 4> s_;
};

} // namespace detail

template <class T>
class PRNG
{
public:
    using Generator = T;

    /// Construct an RNG with seed from system time
    PRNG()
        : PRNG{std::random_device{}()}
    {
    }
    /// Construct an RNG from a specific seed
    PRNG(uint64_t seed);
    /// Construct an RNG from an existing RNG and an optional mask for the new seed (updates the reference)
    template <class S>
    PRNG(PRNG<S>& reference, uint64_t seedMask = 0)
        : PRNG{reference.uniform() ^ seedMask}
    {
    }
    virtual ~PRNG() = default;

    /// Return a value in range [0, max)
    uint64_t uniform() { return generator_(); }
    /// Return a value within the given half-open range
    template <class I, std::enable_if_t<std::is_integral<I>::value, bool> = true>
    I uniform(const Range<I>& range)
    {
        return std::uniform_int_distribution<>{static_cast<int>(range.low()),
                                               static_cast<int>(range.high())}(generator_);
    }
    template <class F, std::enable_if_t<std::is_floating_point<F>::value, bool> = true>
    F uniform(const Range<F>& range)
    {
        return std::uniform_real_distribution<>{static_cast<double>(range.low()),
                                                static_cast<double>(range.high())}(generator_);
    }

    /// Return a value following a normal distribution
    template <class R>
    R gaussian(const R& mean = 0., const R& stddev = 1.)
    {
        return std::normal_distribution<>{mean, stddev}(generator_);
    }

    /// Given a discrete probability distribution through iterator, returns one realisation
    /// @return the index corresponding to the realisation of the input dpd
    template <class P>
    size_t realisation(P&& it, const P& end)
    {
        // Generated value
        const typename P::value_type x = std::uniform_real_distribution<>(0., 1.)(generator_);
        return detail::realise(std::move(it), end, x);
    }

    Generator& generator() { return generator_; }
    const Generator& generator() const { return generator_; }

private:
    Generator generator_;
};

using Mersenne = PRNG<std::mt19937_64>;
using Random = PRNG<detail::Xoshiro256>;

} // namespace rng
} // namespace math
} // namespace galaxias
