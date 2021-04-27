#include "zip_inflater.h"

#include <zlib.h>

namespace vca
{

struct ZipInflater::Impl
{
    Impl(const fs::path& file,
         const size_t max_byte_count,
         const std::string& entry)
    {
    }
};

ZipInflater::ZipInflater(const fs::path& file,
                         const size_t max_byte_count,
                         const std::string& entry)
    : m_impl{std::make_unique<Impl>(file, max_byte_count, entry)}
{
}

ZipInflater::~ZipInflater() = default;

bool
ZipInflater::end() const
{
}

std::string
ZipInflater::next()
{
}

} // namespace vca
