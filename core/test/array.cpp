#include <core/array.h>

#include <catch2/catch_template_test_macros.hpp>

using namespace galaxias;

TEMPLATE_TEST_CASE("Empty array has nullptr", "[array]", uint8_t, int64_t, float, double)
{
    ArrayView<TestType, 1> empty;
    CHECK(empty.data() == nullptr);
    CHECK(empty.size() == 0);
    CHECK(empty.dims() == (typename ArrayView<TestType, 1>::Dims{{0}}));
}

TEMPLATE_TEST_CASE("View on simple array has correct observables", "[array]", uint8_t, int64_t, float, double)
{
    std::vector<TestType> data;
    data.resize(30);
    std::iota(data.begin(), data.end(), 0);

    {
        // Constructors
        ArrayView<TestType, 1> base(data.data(), data.size());

        ArrayView<TestType, 1> copyCtor{base};
        CHECK(copyCtor.data() == data.data());
        CHECK(copyCtor.size() == data.size());

        ArrayView<TestType, 1> moveCtor{std::move(base)};
        CHECK(moveCtor.data() == data.data());
        CHECK(moveCtor.size() == data.size());
    }

    {
        // Copy operators
        ArrayView<TestType, 1> base(data.data(), data.size());

        ArrayView<TestType, 1> copy;
        copy = base;
        CHECK(copy.data() == data.data());
        CHECK(copy.size() == data.size());

        ArrayView<TestType, 1> move;
        move = std::move(base);
        CHECK(move.data() == data.data());
        CHECK(move.size() == data.size());
    }

    // Basic array
    constexpr TestType newVal{127};
    {
        ArrayView<TestType, 1> view(data.data(), data.size());
        CHECK(view.data() == data.data());
        CHECK(view.size() == data.size());
        CHECK(view.dims() == (typename ArrayView<TestType, 1>::Dims{{data.size()}}));
        view.at(3) = newVal;
        CHECK(view[3] == newVal);
    }

    // Const value with offset
    {
        ArrayView<const TestType, 1> view(data.data() + 2, data.size() - 4);
        CHECK(view.data() == &data[2]);
        CHECK(view.size() == data.size() - 4);
        CHECK(view.dims() == (typename ArrayView<TestType, 1>::Dims{{data.size() - 4}}));
        CHECK(view[1] == newVal);
    }
}

// 2d, change dims

TEMPLATE_TEST_CASE("View on 2D arrays has correct observables", "[array]", uint8_t, int64_t, float, double)
{
    std::vector<TestType> data;
    data.resize(30);
    std::iota(data.begin(), data.end(), 0);
    constexpr typename ArrayView<TestType, 2>::Dims baseDims{{6, 5}};

    {
        // Constructors
        ArrayView<TestType, 2> base(data.data(), baseDims);

        ArrayView<TestType, 2> copyCtor{base};
        CHECK(copyCtor.data() == data.data());
        CHECK(copyCtor.size() == data.size());
        CHECK(copyCtor.dims() == baseDims);

        ArrayView<TestType, 2> moveCtor{std::move(base)};
        CHECK(moveCtor.data() == data.data());
        CHECK(moveCtor.size() == data.size());
        CHECK(moveCtor.dims() == baseDims);
    }

    {
        // Copy operators
        ArrayView<TestType, 2> base(data.data(), baseDims);

        ArrayView<TestType, 2> copy;
        copy = base;
        CHECK(copy.data() == data.data());
        CHECK(copy.size() == data.size());
        CHECK(copy.dims() == baseDims);

        ArrayView<TestType, 2> move;
        move = std::move(base);
        CHECK(move.data() == data.data());
        CHECK(move.size() == data.size());
        CHECK(move.dims() == baseDims);
    }

    // Basic array
    constexpr TestType newVal1{127};
    constexpr TestType newVal3{199};
    {
        ArrayView<TestType, 2> view(data.data(), baseDims);
        CHECK(view.data() == data.data());
        CHECK(view.size() == data.size());
        CHECK(view.dims() == baseDims);
        view.at(3) = newVal1;
        view.at({{2, 4}}) = newVal3;
        CHECK(view[3] == newVal1);
        CHECK(view[{{2, 4}}] == newVal3);
    }

    // Const value with offset
    {
        constexpr typename ArrayView<TestType, 2>::Dims newDims{{5, 3}};
        ArrayView<const TestType, 2> view(data.data() + 2, newDims);
        CHECK(view.data() == &data[2]);
        CHECK(view.size() == 5 * 3);
        CHECK(view.dims() == newDims);
        CHECK(view[1] == newVal1);
        CHECK(view[{{4, 0}}] == newVal3);
    }
}
