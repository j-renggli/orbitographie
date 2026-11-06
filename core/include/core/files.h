#pragma once

#include "array.h"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

namespace galaxias
{

class TemporaryFolder
{
public:
    TemporaryFolder();
    TemporaryFolder(const TemporaryFolder&) = delete;
    TemporaryFolder(TemporaryFolder&&) noexcept;
    ~TemporaryFolder();

    TemporaryFolder& operator=(const TemporaryFolder&) = delete;
    TemporaryFolder& operator=(TemporaryFolder&&) noexcept;

    const std::filesystem::path path() const { return path_; }

private:
    void remove();

private:
    std::filesystem::path path_;
};

class TemporaryFile
{
public:
    TemporaryFile(const std::string& extension = "");
    TemporaryFile(const TemporaryFile&) = delete;
    TemporaryFile(TemporaryFile&&) noexcept;
    ~TemporaryFile();

    TemporaryFile& operator=(const TemporaryFile&) = delete;
    TemporaryFile& operator=(TemporaryFile&&) noexcept;

    const std::filesystem::path path() const { return path_; }

private:
    void remove();

private:
    std::filesystem::path path_;
};

class FilesManager
{
public:
    static Owning1DArray<uint32_t> loadFile(const std::filesystem::path& filePath);
};

} // namespace galaxias
