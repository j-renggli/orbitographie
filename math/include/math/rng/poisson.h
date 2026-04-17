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
std::vector<T> generatePoisson(T mean, size_t count)
{
    T current = std::exp(-mean);
    std::vector<T> values(count);
    for (size_t i = 0; i < count; ++i)
    {
        values[i] = current;
        current *= mean / static_cast<T>(i + 1);
    }

    return values;
}

} // namespace rng
} // namespace math
} // namespace galaxias
