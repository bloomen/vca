#include "zip_inflater.h"

#include <regex>
#include <zip/zip.h>

#include "logging.h"

namespace vca
{

namespace
{

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
        , m_regex{entry}
    {
        VCA_CHECK(m_file) << "Could not open zip file: " << file;
        m_entry_count = static_cast<int>(zip_entries_total(m_file.get()));
    }

    bool
    process_next_entry(Buffer& buffer)
    {
        if (m_current_entry >= m_entry_count)
        {
            return false;
        }
        VCA_CHECK(!zip_entry_openbyindex(m_file.get(), m_current_entry))
            << "Could not open zip entry at: " << m_current_entry;
        const std::string name = zip_entry_name(m_file.get());
        if (std::regex_match(name, m_regex))
        {
            zip_entry_extract(m_file.get(), on_extract, &buffer);
        }
        zip_entry_close(m_file.get());
        ++m_current_entry;
        return true;
    }

    VCA_DELETE_COPY(ZipFile)
    VCA_DELETE_MOVE(ZipFile)

private:
    std::unique_ptr<zip_t, ZipDeleter> m_file;
    std::regex m_regex;
    int m_entry_count = 0;
    int m_current_entry = 0;
};

} // namespace

struct ZipInflater::Impl
{
    Impl(const fs::path& file,
         const size_t max_byte_count,
         const std::string& entry)
        : zip_file{file, entry}
    {
        buffer.max_byte_count = max_byte_count;
        while (buffer.data.size() <= max_byte_count &&
               zip_file.process_next_entry(buffer))
            ;
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
