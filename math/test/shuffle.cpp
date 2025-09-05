#include <math/rng/shuffle.h>

#include <catch2/catch_test_macros.hpp>
#include <math/rng/prng.h>

using namespace galaxias;
using namespace math;

namespace test
{

class ConstNG
{
public:
    ConstNG(uint64_t val)
        : value_{val}
    {
    }

    uint64_t uniform(const math::Range<uint64_t>&) { return value_; }

private:
    const uint64_t value_;
};

class DecreaseNG
{
public:
    DecreaseNG(uint64_t initial)
        : value_{initial}
    {
    }

    uint64_t uniform(const math::Range<uint64_t>&)
    {
        auto tmp = value_;
        value_ = std::max(2ul, value_) - 2;
        return tmp;
    }

private:
    uint64_t value_;
};

} // namespace test

namespace
{
const std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
}

TEST_CASE("Shuffle that does nothing")
{
    test::ConstNG zero{0};
    auto values2 = values;
    rng::shuffle(values2.begin(), values2.end(), zero);
    CHECK(values2 == values);
}

TEST_CASE("Shuffle that shifts by one")
{
    const size_t size = values.size();
    test::ConstNG one{1};
    auto values2 = values;
    rng::shuffle(values2.begin(), values2.end(), one);
    CHECK(values2[size - 1] == 0);
    for (size_t i = 1; i < size; ++i)
    {
        INFO(i);
        CHECK(values2[i - 1] == values[i]);
    }
}

TEST_CASE("Shuffle that inverts the vector")
{
    const size_t size = values.size();
    test::DecreaseNG dec{size - 1};
    auto values2 = values;
    rng::shuffle(values2.begin(), values2.end(), dec);
    for (size_t i = 0; i < size; ++i)
    {
        INFO(i);
        CHECK(values2[i] == values[size - i - 1]);
    }
}

TEST_CASE("Shuffle with an arbitrary RNG")
{
    auto values2 = values;
    math::rng::Random random(1);
    rng::shuffle(values2.begin(), values2.end(), random);
    const std::vector<int> expected = {8, 7, 3, 9, 11, 10, 17, 0, 14, 16, 2, 5, 1, 12, 4, 13, 20, 15, 19, 6, 18};
    CHECK(values2 == expected);
}
