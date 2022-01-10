#include "zip_inflater.h"

#include <zip/zip.h>

#include "logging.h"

namespace vca
{

namespace
{

struct ZipDeleter
{
    void
    operator()(zip_t* file) const
    {
        if (file)
        {
            zip_close(file);
        }
    }
};

class ZipFile
{
public:
    ZipFile(const fs::path& file, const std::string& entry)
        : m_file{zip_open(file.u8string().c_str(), 0, 'r'), ZipDeleter{}}
    {
        VCA_CHECK(m_file) << "Could not open zip file: " << file;
        VCA_CHECK(!zip_entry_open(m_file.get(), entry.c_str()))
            << "Could not open zip entry in: " << file;
    }

    ~ZipFile()
    {
        zip_entry_close(m_file.get());
    }

    VCA_DELETE_COPY(ZipFile)
    VCA_DELETE_MOVE(ZipFile)

    zip_t*
    handle() const
    {
        return m_file.get();
    }

private:
    std::unique_ptr<zip_t, ZipDeleter> m_file;
};

struct Buffer
{
    std::string data;
    size_t max_byte_count;
};

size_t
on_extract(void* const arg,
           const uint64_t,
           const void* const data_void,
           const size_t size)
{
    const auto buffer = static_cast<Buffer*>(arg);
    const auto data = static_cast<const char*>(data_void);
    buffer->data.insert(buffer->data.begin(), data, data + size);
    if (buffer->data.size() >= buffer->max_byte_count)
    {
        return 0;
    }
    return size;
}

} // namespace

struct ZipInflater::Impl
{
    Impl(const fs::path& file,
         const size_t max_byte_count,
         const std::string& entry)
        : zip_file{file, entry}
    {
        buffer.max_byte_count = max_byte_count;
        zip_entry_extract(zip_file.handle(), on_extract, &buffer);
    }

    ZipFile zip_file;
    Buffer buffer;
};

ZipInflater::ZipInflater(const fs::path& file,
                         const size_t max_byte_count,
                         const std::string& entry)
    : m_impl{std::make_unique<Impl>(file, max_byte_count, entry)}
{
}

ZipInflater::~ZipInflater() = default;

const std::string&
ZipInflater::get() const&
{
    return m_impl->buffer.data;
}

std::string&&
ZipInflater::get() &&
{
    return std::move(m_impl->buffer.data);
}

} // namespace vca
