#include <cmath>

namespace galaxias
{
namespace math
{

template <class T>
bool Range<T>::includes(const T& value) const
{
    return value >= lo_ && value <= hi_;
}

template <class T>
void Range<T>::clamp(T& value) const
{
    if (value > hi_)
    {
        value = hi_;
    }
    else if (value < lo_)
    {
        value = lo_;
    }
}

template <class T>
T Range<T>::clamp(const T& value) const
{
    T clamped{value};
    clamp(clamped);
    return clamped;
}

template <class T>
void Range<T>::modulo(T& value) const
{
    if (value >= lo_ && value < hi_)
    {
        return;
    }

    const double diff = hi_ - lo_;
    double n = 0;
    if (value > hi_)
    {
        n = std::ceil((value - static_cast<double>(hi_)) / diff);
    }
    else if (value < lo_)
    {
        n = std::floor((value - static_cast<double>(lo_)) / diff);
    }

    value -= static_cast<T>(n * diff);

    if (value == hi_)
    {
        value = lo_;
    }
}

template <class T>
T Range<T>::modulo(const T& value) const
{
    T modulated{value};
    modulo(modulated);
    return modulated;
}

} // namespace math
} // namespace galaxias
