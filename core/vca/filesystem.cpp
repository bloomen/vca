#include "filesystem.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <cryptopp/crc.h>
#include <sago/platform_folders.h>

#include "utils.h"

namespace vca
{

Fingerprint
Fingerprint::from_path(const Path& f)
{
    VCA_CHECK(f.is_file());
    // std::filesystem doesn't have a file_time to time_t conversion :(
    const auto last_write_time = static_cast<uint64_t>(
        boost::filesystem::last_write_time(f.to_narrow()));
    auto file = make_ifstream(f, std::ios_base::binary);
    VCA_CHECK(file.good());
    return from_stream(file, f.size(), last_write_time);
}

Fingerprint
Fingerprint::from_stream(std::ifstream& is,
                         uint64_t size,
                         uint64_t last_write_time)
{
    Fingerprint fp;
    fp.m_size = size;
    fp.m_last_write_time = last_write_time;

    constexpr size_t limit = 8192;
    if (fp.m_size <= sizeof(fp.m_crc))
    {
        // tiny file
        is.read(reinterpret_cast<char*>(fp.m_crc.data()), fp.m_size);
        VCA_CHECK(!is.bad());
    }
    else if (fp.m_size < limit)
    {
        // small file
        CryptoPP::CRC32 hash;
        unsigned char buf[limit];
        int32_t value;

        is.read((char*)buf, fp.m_size);
        VCA_CHECK(!is.bad());

        for (size_t i = 0; i < fp.m_crc.size(); i++)
        {
            const int begin = static_cast<int>(i * fp.m_size / fp.m_crc.size());
            const int end =
                static_cast<int>((i + 1) * fp.m_size / fp.m_crc.size());
            hash.Update(buf + begin, end - begin);
            hash.Final(reinterpret_cast<unsigned char*>(&value));
            fp.m_crc[i] = value;
        }
    }
    else
    {
        // larger file, 4 sparse crcs
        CryptoPP::CRC32 hash;
        unsigned char block[4 * sizeof(fp.m_crc)];
        int32_t value;
        const auto blocks = limit / (sizeof(block) * fp.m_crc.size());

        for (size_t i = 0; i < fp.m_crc.size(); i++)
        {
            for (size_t j = 0; j < blocks; j++)
            {
                is.seekg((fp.m_size - sizeof(block)) * (i * blocks + j) /
                         (fp.m_crc.size() * blocks - 1));
                is.read(reinterpret_cast<char*>(block), sizeof(block));
                VCA_CHECK(!is.bad());

                hash.Update(block, sizeof(block));
            }

            hash.Final(reinterpret_cast<unsigned char*>(&value));
            fp.m_crc[i] = value;
        }
    }

    return fp;
}

std::vector<unsigned char>
Fingerprint::serialize() const
{
    std::vector<unsigned char> d(sizeof(Fingerprint));
    std::memcpy(d.data(),
                reinterpret_cast<const unsigned char*>(this),
                sizeof(Fingerprint));
    return d;
}

Fingerprint
Fingerprint::deserialize(const std::vector<unsigned char>& d)
{
    Fingerprint fp;
    std::memcpy(
        reinterpret_cast<unsigned char*>(&fp), d.data(), sizeof(Fingerprint));
    return fp;
}

bool
operator==(const Fingerprint& l, const Fingerprint& r)
{
    if (l.m_size != r.m_size)
    {
        return false;
    }
    if (l.m_last_write_time != r.m_last_write_time)
    {
        return false;
    }
    if (l.m_crc != r.m_crc)
    {
        return false;
    }
    return true;
}

bool
Path::is_parent_of(const Path& a_child) const
{
    if (*this == a_child)
    {
        return false;
    }
    auto parent = *this;
    if (!parent.is_absolute())
    {
        parent = parent.absolute();
    }
    auto child = a_child;
    if (!child.is_absolute())
    {
        child = child.absolute();
    }
    auto parent_it = parent.m_path.begin();
    auto child_it = child.m_path.begin();
    while (parent_it != parent.m_path.end() && child_it != child.m_path.end())
    {
        if (*parent_it != *child_it)
        {
            return false;
        }
        ++parent_it;
        ++child_it;
    }
    return child_it != child.m_path.end();
}

Path
user_config_dir()
{
    return Path{sago::getConfigHome()};
}

Path
user_documents_dir()
{
    return Path{sago::getDocumentsFolder()};
}

std::string
read_text(std::istream& f, const size_t max_byte_count)
{
    VCA_CHECK(f.good());
    std::string data(max_byte_count, 0);
    f.read(data.data(), data.size());
    const auto byte_count = f.gcount();
    data.resize(byte_count);
    return data;
}

} // namespace vca
