#include "zip_inflater.h"

// TODO: Use zlib for reading chunk by chunk
// #include <zlib.h>

#include <zip.h>

#include "logging.h"

namespace vca
{

struct ZipInflater::Impl
{
    Impl(const fs::path& file,
         const size_t max_byte_count,
         const std::string& entry)
    {
        auto zip_file = zip_open(file.u8string().c_str(), 0, 'r');
        VCA_CHECK(zip_file) << "Could not open zip file: " << file;
        VCA_CHECK(!zip_entry_open(zip_file, entry.c_str()))
            << "Could not open zip entry in: " << file;

        void* buf = nullptr;
        size_t size;
        // TODO: Replace with reading chunk by chunk
        VCA_CHECK(zip_entry_read(zip_file, &buf, &size) >= 0)
            << "Read from entry failed: " << file;

        zip_entry_close(zip_file);
        zip_close(zip_file);
        if (size > max_byte_count)
        {
            size = max_byte_count;
        }
        content = {static_cast<char*>(buf), static_cast<char*>(buf) + size};
        free(buf);
    }

    std::string content;
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
    return m_impl->content.empty();
}

std::string
ZipInflater::next()
{
    return std::move(m_impl->content);
}

} // namespace vca
