#include "file_watcher.h"

#include <efsw/efsw.hpp>

namespace vca
{

struct FileWatcher::Impl : public efsw::FileWatchListener
{
    Impl(const AppConfig& app_config,
         const UserConfig& user_config,
         UserDb& user_db,
         const FileProcessor& file_processor)
        : app_config{app_config}
        , user_config{user_config}
        , user_db{user_db}
        , file_processor{file_processor}
    {
        watch = file_watcher.addWatch(
            user_config.root_dir().u8string(), this, true);
        file_watcher.watch();
    }

    ~Impl()
    {
        file_watcher.removeWatch(watch);
    }

    void
    handleFileAction(const efsw::WatchID,
                     const std::string& dir,
                     const std::string& filename,
                     const efsw::Action action,
                     const std::string oldFilename = "")
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
                user_db.update_file(path, contents);
            }
            break;
        }
        case efsw::Actions::Delete:
        {
            if (app_config.matches_ext(path))
            {
                user_db.remove_file(path);
            }
            break;
        }
        case efsw::Actions::Moved:
        {
            const auto old_path = fs::u8path(dir) / fs::u8path(oldFilename);
            if (app_config.matches_ext(old_path))
            {
                if (app_config.matches_ext(path))
                {
                    user_db.move_file(old_path, path);
                }
                else
                {
                    user_db.remove_file(old_path);
                }
            }
            break;
        }
        default:
            VCA_CHECK(false);
        }
    }

    const AppConfig& app_config;
    const UserConfig& user_config;
    UserDb& user_db;
    const FileProcessor& file_processor;
    efsw::FileWatcher file_watcher;
    efsw::WatchID watch;
};

FileWatcher::FileWatcher(const AppConfig& app_config,
                         const UserConfig& user_config,
                         UserDb& user_db,
                         const FileProcessor& file_processor)
    : m_impl{std::make_unique<Impl>(app_config,
                                    user_config,
                                    user_db,
                                    file_processor)}
{
}

FileWatcher::~FileWatcher() = default;

} // namespace vca
