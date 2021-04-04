#include "file_watcher.h"

#include <efsw/efsw.hpp>

#include <vca/logging.h>

namespace vca
{

struct FileWatcher::Impl final : public efsw::FileWatchListener,
                                 public UserConfig::Observer
{
    Impl(CommandQueue& commands,
         const AppConfig& app_config,
         UserConfig& user_config,
         UserDb& user_db,
         const FileProcessor& file_processor)
        : commands{commands}
        , app_config{app_config}
        , user_config{user_config}
        , root_dir{user_config.root_dir()}
        , user_db{user_db}
        , file_processor{file_processor}
    {
        user_config.add_observer(*this);
        if (!fs::exists(root_dir))
        {
            VCA_ERROR << "root_dir does not exist: " << root_dir;
            return;
        }
        watch = file_watcher.addWatch(root_dir.u8string(), this, true);
        VCA_CHECK(watch > 0);
        file_watcher.watch();
    }

    ~Impl()
    {
        file_watcher.removeWatch(watch);
        user_config.remove_observer(*this);
    }

    void
    user_config_changed(const UserConfig&) override
    {
        if (root_dir != user_config.root_dir())
        {
            root_dir = user_config.root_dir();
            VCA_INFO << "Reloading file watcher: " << root_dir;
            user_db.create(root_dir);
            file_watcher.removeWatch(watch);
            watch = file_watcher.addWatch(root_dir.u8string(), this, true);
            file_watcher.watch();
        }
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
            if (app_config.matches_ext(path))
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
            if (app_config.matches_ext(path))
            {
                commands.push([this, path] { user_db.remove_file(path); });
            }
            break;
        }
        case efsw::Actions::Moved:
        {
            auto old_path = fs::u8path(dir) / fs::u8path(old_filename);
            if (app_config.matches_ext(old_path))
            {
                if (app_config.matches_ext(path))
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
    const AppConfig& app_config;
    UserConfig& user_config;
    fs::path root_dir;
    UserDb& user_db;
    const FileProcessor& file_processor;
    efsw::FileWatcher file_watcher;
    efsw::WatchID watch;
}; // namespace vca

FileWatcher::FileWatcher(CommandQueue& commands,
                         const AppConfig& app_config,
                         UserConfig& user_config,
                         UserDb& user_db,
                         const FileProcessor& file_processor)
    : m_impl{std::make_unique<Impl>(commands,
                                    app_config,
                                    user_config,
                                    user_db,
                                    file_processor)}
{
}

FileWatcher::~FileWatcher() = default;

} // namespace vca
