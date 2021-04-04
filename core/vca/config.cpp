#include "config.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <mutex>

#include <efsw/efsw.hpp>

#include "json.h"
#include "logging.h"
#include "utils.h"

using json = nlohmann::json;

namespace vca
{

struct UserConfig::Impl : public efsw::FileWatchListener
{
    Impl(UserConfig& user_config, fs::path path)
        : user_config{user_config}
        , path{make_path(std::move(path))}
        , file_lock{this->path.parent_path()}
    {
        std::lock_guard<FileLock> lock{file_lock};
        if (fs::exists(this->path))
        {
            read();
        }
        else
        {
            populate();
            write();
        }
        watch = file_watcher.addWatch(
            this->path.parent_path().u8string(), this, true);
        VCA_CHECK(watch > 0);
        file_watcher.watch();
    }

    ~Impl()
    {
        file_watcher.removeWatch(watch);
    }

    static fs::path
    make_path(fs::path path)
    {
        fs::create_directories(path.parent_path());
        return path;
    }

    void
    read()
    {
        json j;
        {
            std::ifstream file{path};
            VCA_CHECK(file);
            j = json::parse(file);
            VCA_CHECK(!file.bad());
        }
        root_dir = fs::u8path(j["root_dir"].get<std::string>());
    }

    void
    populate()
    {
        root_dir = user_documents_dir();
        if (!fs::exists(root_dir))
        {
            root_dir = root_dir.parent_path();
            VCA_CHECK(fs::exists(root_dir))
                << "root dir does not exist: " << root_dir;
        }
    }

    void
    write() const
    {
        json j;
        j["root_dir"] = root_dir.u8string();
        std::ofstream{path} << j;
    }

    void
    handleFileAction(const efsw::WatchID,
                     const std::string&,
                     const std::string& filename,
                     const efsw::Action action,
                     const std::string old_filename)
    {
        switch (action)
        {
        case efsw::Actions::Add:
        case efsw::Actions::Modified:
        {
            if (fs::u8path(filename) != path.filename())
            {
                return;
            }
            VCA_INFO << "User config modified";
            std::lock_guard<FileLock> lock{file_lock};
            read();
            if (!fs::exists(root_dir))
            {
                VCA_ERROR << "root_dir does not exist: " << root_dir;
                break;
            }
            for (auto& observer : observers)
            {
                observer->user_config_changed(user_config);
            }
            break;
        }
        case efsw::Actions::Delete:
        {
            if (fs::u8path(filename) != path.filename())
            {
                return;
            }
            VCA_INFO << "User config deleted";
            std::lock_guard<FileLock> lock{file_lock};
            populate();
            write();
            break;
        }
        case efsw::Actions::Moved:
        {
            if (fs::u8path(old_filename) != path.filename())
            {
                return;
            }
            VCA_INFO << "User config moved";
            std::lock_guard<FileLock> lock{file_lock};
            populate();
            write();
            break;
        }
        default:
            VCA_CHECK(false);
        }
    }

    UserConfig& user_config;
    fs::path path;
    FileLock file_lock;
    fs::path root_dir;
    std::set<UserConfig::Observer*> observers;
    efsw::FileWatcher file_watcher;
    efsw::WatchID watch;
};

UserConfig::UserConfig(fs::path path)
    : m_impl{std::make_unique<Impl>(*this, std::move(path))}
{
}

UserConfig::~UserConfig() = default;

const fs::path&
UserConfig::root_dir() const
{
    return m_impl->root_dir;
}

void
UserConfig::add_observer(Observer& observer)
{
    m_impl->observers.emplace(&observer);
}

void
UserConfig::remove_observer(Observer& observer)
{
    m_impl->observers.erase(&observer);
}

const std::set<std::string>&
AppConfig::extensions() const
{
    return m_extensions;
}

bool
AppConfig::matches_ext(const fs::path& path) const
{
    if (!fs::is_regular_file(path))
    {
        return false;
    }
    auto ext = path.extension().u8string();
    // TODO: Does this work with UTF-8?
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return m_extensions.count(ext) > 0;
}

} // namespace vca
