#include "file_watcher.h"

#include <map>

#include <efsw/efsw.hpp>

#include <vca/logging.h>

namespace vca
{

namespace
{

struct Watcher : efsw::FileWatchListener
{
    Watcher(CommandQueue& commands,
            fs::path root_dir,
            UserDb& user_db,
            const FileProcessor& file_processor)
        : commands{commands}
        , root_dir{std::move(root_dir)}
        , user_db{user_db}
        , file_processor{file_processor}
    {
        VCA_CHECK(fs::exists(this->root_dir))
            << "root_dir does not exist: " << this->root_dir;
        VCA_INFO << "Adding watch for: " << this->root_dir;
        watch = file_watcher.addWatch(this->root_dir.u8string(), this, true);
        VCA_CHECK(watch > 0);
        file_watcher.watch();
    }

    ~Watcher()
    {
        file_watcher.removeWatch(watch);
    }

    // called from file watcher thread
    void
    handleFileAction(const efsw::WatchID,
                     const std::string& dir,
                     const std::string& filename,
                     const efsw::Action action,
                     const std::string old_filename = "")
    {
        const auto path = fs::u8path(dir) / fs::u8path(filename);
        switch (action)
        {
        case efsw::Actions::Add:
        case efsw::Actions::Modified:
        {
            if (fs::is_regular_file(path))
            {
                vca::FileContents contents;
                contents.words = file_processor.process(path);
                commands.push([this, path, contents = std::move(contents)] {
                    user_db.update_file(path, contents);
                });
            }
            break;
        }
        case efsw::Actions::Delete:
        {
            if (fs::is_regular_file(path))
            {
                commands.push([this, path] { user_db.remove_file(path); });
            }
            break;
        }
        case efsw::Actions::Moved:
        {
            auto old_path = fs::u8path(dir) / fs::u8path(old_filename);
            if (fs::is_regular_file(old_path))
            {
                if (fs::is_regular_file(path))
                {
                    commands.push([this, path, old_path = std::move(old_path)] {
                        user_db.move_file(old_path, path);
                    });
                }
            }
            else
            {
                commands.push([this, old_path = std::move(old_path)] {
                    user_db.remove_file(old_path);
                });
            }
            break;
        }
        default:
            VCA_CHECK(false);
        }
    }

    CommandQueue& commands;
    fs::path root_dir;
    UserDb& user_db;
    const FileProcessor& file_processor;
    efsw::FileWatcher file_watcher;
    efsw::WatchID watch;
};

} // namespace

struct FileWatcher::Impl final : UserConfig::Observer
{
    Impl(CommandQueue& commands,
         UserConfig& user_config,
         UserDb& user_db,
         const FileProcessor& file_processor)
        : commands{commands}
        , user_config{user_config}
        , user_db{user_db}
        , file_processor{file_processor}
    {
        user_config.add_observer(*this);
        user_config_changed(user_config);
    }

    ~Impl()
    {
        user_config.remove_observer(*this);
    }

    void
    user_config_changed(const UserConfig&) override
    {
        // find dirs that don't need watching anymore
        std::set<fs::path> trash;
        for (const auto& [dir, s] : watchers)
        {
            if (user_config.root_dirs().find(dir) ==
                user_config.root_dirs().end())
            {
                trash.emplace(dir);
            }
        }
        for (const auto& dir : trash)
        {
            watchers.erase(dir);
            user_db.remove_root_dir(dir);
        }
        // add new dirs that need watching
        for (const auto& dir : user_config.root_dirs())
        {
            if (watchers.find(dir) == watchers.end())
            {
                user_db.add_root_dir(dir);
                try
                {
                    watchers.emplace(
                        dir,
                        std::make_unique<Watcher>(
                            commands, dir, user_db, file_processor));
                }
                catch (...)
                {
                }
            }
        }
    }

    CommandQueue& commands;
    UserConfig& user_config;
    UserDb& user_db;
    const FileProcessor& file_processor;
    std::map<fs::path, std::unique_ptr<Watcher>> watchers;
}; // namespace vca

FileWatcher::FileWatcher(CommandQueue& commands,
                         UserConfig& user_config,
                         UserDb& user_db,
                         const FileProcessor& file_processor)
    : m_impl{std::make_unique<Impl>(commands,
                                    user_config,
                                    user_db,
                                    file_processor)}
{
}

FileWatcher::~FileWatcher() = default;

} // namespace vca
