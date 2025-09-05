#include <core/files.h>

#include <catch2/catch_test_macros.hpp>

#include <fstream>

using namespace galaxias;

TEST_CASE("Temporary folder is created and destructed")
{
    std::filesystem::path tmpPath;

    // Empty folder
    {
        TemporaryFolder tmp;
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
    }
    CHECK_FALSE(exists(tmpPath));

    // Non-empty folder
    {
        TemporaryFolder tmp;
        CHECK(tmpPath != tmp.path());
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));

        const auto file = tmpPath / "file.txt";
        std::ofstream output(file);
        CHECK(exists(file));
    }
    CHECK_FALSE(exists(tmpPath));

    // Move constructor
    {
        TemporaryFolder tmp;
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
        {
            TemporaryFolder tmp2(std::move(tmp));
            CHECK(tmp.path() != tmpPath);
            CHECK(tmp2.path() == tmpPath);
            CHECK(exists(tmpPath));
        }
        CHECK_FALSE(exists(tmpPath));
    }

    // Move operator
    {
        TemporaryFolder tmp;
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
        {
            TemporaryFolder tmp2;
            CHECK(tmp2.path() != tmp.path());
            tmp2 = std::move(tmp);
            CHECK(tmp.path() != tmpPath);
            CHECK(tmp2.path() == tmpPath);
            CHECK(exists(tmpPath));
        }
        CHECK_FALSE(exists(tmpPath));
    }
}

TEST_CASE("Temporary file is created and destructed")
{
    std::filesystem::path tmpPath;

    // Extensionless file
    {
        TemporaryFile tmp;
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
    }
    CHECK_FALSE(exists(tmpPath));

    // File with extension
    {
        TemporaryFile tmp{".txt"};
        CHECK(tmpPath != tmp.path());
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
        CHECK(tmpPath.extension() == ".txt");
    }
    CHECK_FALSE(exists(tmpPath));

    // Move constructor
    {
        TemporaryFile tmp;
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
        {
            TemporaryFile tmp2(std::move(tmp));
            CHECK(tmp.path() != tmpPath);
            CHECK(tmp2.path() == tmpPath);
            CHECK(exists(tmpPath));
        }
        CHECK_FALSE(exists(tmpPath));
    }

    // Move operator
    {
        TemporaryFile tmp;
        tmpPath = tmp.path();
        CHECK(exists(tmpPath));
        {
            TemporaryFile tmp2;
            CHECK(tmp2.path() != tmp.path());
            tmp2 = std::move(tmp);
            CHECK(tmp.path() != tmpPath);
            CHECK(tmp2.path() == tmpPath);
            CHECK(exists(tmpPath));
        }
        CHECK_FALSE(exists(tmpPath));
    }
}
