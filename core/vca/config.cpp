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

namespace
{

struct Keys
{
    constexpr static const char* const root_dirs = "root_dirs";
};

} // namespace

struct UserConfig::Impl : public efsw::FileWatchListener
{
    Impl(CommandQueue& commands, UserConfig& user_config, fs::path path)
        : commands{commands}
        , user_config{user_config}
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
    normalize_root_dirs()
    {
        std::set<fs::path> trash;
        for (const auto& dir : root_dirs)
        {
            for (const auto& dir_ref : root_dirs)
            {
                if (dir != dir_ref)
                {
                    if (is_parent_of(dir_ref, dir))
                    {
                        VCA_ERROR << dir << " is a child dir of: " << dir_ref;
                        trash.emplace(dir);
                    }
                }
            }
        }
        for (const auto& dir : trash)
        {
            root_dirs.erase(dir);
        }
    }

    bool
    valid_root_dir(const fs::path& path)
    {
        if (!fs::exists(path))
        {
            VCA_ERROR << "root_dir does not exist: " << path;
            return false;
        }
        if (!path.is_absolute())
        {
            VCA_ERROR << "root_dir is not absolute: " << path;
            return false;
        }
        return true;
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
        root_dirs.clear();
        for (const auto& dir : j[Keys::root_dirs])
        {
            auto path = fs::u8path(dir.get<std::string>());
            if (!valid_root_dir(path))
            {
                continue;
            }
            root_dirs.emplace(std::move(path));
        }
        normalize_root_dirs();
    }

    void
    populate()
    {
        auto dir = user_documents_dir();
        if (!fs::exists(dir))
        {
            dir = dir.parent_path();
            VCA_CHECK(valid_root_dir(dir));
        }
        root_dirs.emplace(std::move(dir));
    }

    void
    write() const
    {
        json j;
        auto dirs = json::array();
        for (const auto& dir : root_dirs)
        {
            dirs.push_back(dir.u8string());
        }
        j[Keys::root_dirs] = dirs;
        std::ofstream{path} << j;
    }

    // called from file watcher thread
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
            commands.push([this, filename] {
                if (fs::u8path(filename) != path.filename())
                {
                    return;
                }
                VCA_INFO << "User config modified";
                std::lock_guard<FileLock> lock{file_lock};
                read();
                for (auto& observer : observers)
                {
                    observer->user_config_changed(user_config);
                }
            });
            break;
        }
        case efsw::Actions::Delete:
        {
            commands.push([this, filename] {
                if (fs::u8path(filename) != path.filename())
                {
                    return;
                }
                VCA_INFO << "User config deleted";
                std::lock_guard<FileLock> lock{file_lock};
                populate();
                write();
            });
            break;
        }
        case efsw::Actions::Moved:
        {
            commands.push([this, filename, old_filename] {
                if (fs::u8path(old_filename) != path.filename())
                {
                    return;
                }
                VCA_INFO << "User config moved";
                std::lock_guard<FileLock> lock{file_lock};
                populate();
                write();
            });
            break;
        }
        default:
            VCA_CHECK(false);
        }
    }

    CommandQueue& commands;
    UserConfig& user_config;
    fs::path path;
    FileLock file_lock;
    std::set<fs::path> root_dirs;
    std::set<UserConfig::Observer*> observers;
    efsw::FileWatcher file_watcher;
    efsw::WatchID watch;
};

UserConfig::UserConfig(CommandQueue& commands, fs::path path)
    : m_impl{std::make_unique<Impl>(commands, *this, std::move(path))}
{
}

UserConfig::~UserConfig() = default;

const std::set<fs::path>&
UserConfig::root_dirs() const
{
    return m_impl->root_dirs;
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

} // namespace vca
