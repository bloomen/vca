#include "file_scanner.h"

#include <atomic>
#include <thread>

#include <vca/logging.h>
#include <vca/time.h>

namespace vca
{

struct FileScanner::Impl : public UserConfig::Observer
{
    Impl(CommandQueue& commands,
         UserConfig& user_config,
         UserDb& user_db,
         const FileProcessor& file_processor)
        : commands{commands}
        , user_config{user_config}
        , root_dir{user_config.root_dir()}
        , user_db{user_db}
        , file_processor{file_processor}
        , thread{[this] { scan(); }}
    {
        user_config.add_observer(*this);
    }

    ~Impl()
    {
        stop();
        user_config.remove_observer(*this);
    }

    void
    user_config_changed(const UserConfig&) override
    {
        if (root_dir != user_config.root_dir())
        {
            root_dir = user_config.root_dir();
            VCA_INFO << "Reloading file scanner: " << root_dir;
            stop();
            user_db.create(root_dir);
            done = false;
            thread = std::thread{[this] { scan(); }};
        }
    }

    // called from scan thread
    void
    scan()
    {
        try
        {
            if (!fs::exists(root_dir))
            {
                VCA_ERROR << "root_dir does not exist: " << root_dir;
                return;
            }
            VCA_INFO << "Scanning: " << root_dir;
            Timer timer;
            for (const auto& p : fs::recursive_directory_iterator{root_dir})
            {
                if (done)
                {
                    break;
                }
                auto path = p.path();
                if (fs::is_regular_file(path))
                {
                    vca::FileContents contents;
                    contents.words = file_processor.process(path);
                    commands.push([this,
                                   path = std::move(path),
                                   contents = std::move(contents)] {
                        user_db.update_file(path, contents);
                    });
                }
            }
            VCA_INFO << "Scanning finished: " << root_dir;
            VCA_INFO << "Scanning took: " << us_to_s(timer.us()) << " s";
        }
        catch (const std::exception& e)
        {
            VCA_EXCEPTION(e) << e.what();
        }
        catch (...)
        {
            VCA_ERROR << "Unknown exception";
        }
    }

    void
    stop()
    {
        done = true;
        thread.join();
    }

    CommandQueue& commands;
    UserConfig& user_config;
    fs::path root_dir;
    UserDb& user_db;
    const FileProcessor& file_processor;
    std::atomic<bool> done{false};
    std::thread thread;
}; // namespace vca

FileScanner::FileScanner(CommandQueue& commands,
                         UserConfig& user_config,
                         UserDb& user_db,
                         const FileProcessor& file_processor)
    : m_impl{std::make_unique<Impl>(commands,
                                    user_config,
                                    user_db,
                                    file_processor)}
{
}

FileScanner::~FileScanner() = default;

} // namespace vca
