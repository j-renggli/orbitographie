#include <core/array.h>

#include <catch2/catch_template_test_macros.hpp>

using namespace galaxias;

TEMPLATE_TEST_CASE("Empty owning array has nullptr", "[array]", uint8_t, int64_t, float, double)
{
    Owning1DArray<TestType> owner;
    CHECK(owner.data() == nullptr);
    CHECK(owner.capacity() == 0);
    CHECK(owner.size() == 0);
    CHECK(owner.dims() == (typename Owning1DArray<TestType>::Dims{{0}}));

    // Which can be reserved
    owner.reserve(5);
    auto data = owner.data();
    CHECK(data != nullptr);
    CHECK(owner.capacity() == 5);
    CHECK(owner.size() == 0);

    // And resized
    owner.resize({{3}});
    CHECK(owner.data() == data);
    CHECK(owner.capacity() == 5);
    CHECK(owner.size() == 3);
    for (size_t i = 0; i < 3; ++i)
    {
        owner[i] = static_cast<TestType>(25 + i);
    }

    // And resize keeps data
    owner.resize({{7}});
    CHECK(owner.data() != data);
    CHECK(owner.capacity() == 7);
    CHECK(owner.size() == 7);
    for (size_t i = 0; i < 3; ++i)
    {
        CHECK(owner[i] == static_cast<TestType>(25 + i));
    }
}

TEMPLATE_TEST_CASE("View on simple owning array has correct observables", "[array]", uint8_t, int64_t, float, double)
{
    constexpr size_t size{8};
    std::array<size_t, 1> dims{{size}};

    {
        // Constructors
        Owning1DArray<TestType> base(dims);
        std::iota(base.data(), base.data() + size, 0);

        Owning1DArray<TestType> copyCtor{base};
        CHECK(copyCtor.capacity() == size);
        CHECK(copyCtor.size() == size);
        CHECK(copyCtor.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(copyCtor[i] == static_cast<TestType>(i));
        }

        Owning1DArray<TestType> moveCtor{std::move(base)};
        CHECK(moveCtor.capacity() == size);
        CHECK(moveCtor.size() == size);
        CHECK(moveCtor.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(moveCtor[i] == static_cast<TestType>(i));
        }
        CHECK(base.data() == nullptr);
    }

    {
        // Copy operators
        Owning1DArray<TestType> base(dims);
        std::iota(base.data(), base.data() + size, 0);

        Owning1DArray<TestType> copy;
        copy = base;
        CHECK(copy.capacity() == size);
        CHECK(copy.size() == size);
        CHECK(copy.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(copy[i] == static_cast<TestType>(i));
        }

        Owning1DArray<TestType> move;
        move = std::move(base);
        CHECK(move.capacity() == size);
        CHECK(move.size() == size);
        CHECK(move.dims() == dims);
        for (size_t i = 0; i < size; ++i)
        {
            CHECK(move[i] == static_cast<TestType>(i));
        }
    }
}
