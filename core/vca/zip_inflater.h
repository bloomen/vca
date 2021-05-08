#pragma once

#include <memory>

#include "filesystem.h"
#include "utils.h"

namespace vca
{

class ZipInflater
{
public:
    ZipInflater(const fs::path& file,
                size_t max_byte_count,
                const std::string& entry);

    VCA_DELETE_COPY(ZipInflater)
    VCA_DEFAULT_MOVE(ZipInflater)

    ~ZipInflater();

    const std::string&
    get() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
