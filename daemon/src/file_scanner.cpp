#include "file_scanner.h"

#include <atomic>
#include <map>
#include <thread>

#include <vca/logging.h>
#include <vca/time.h>

namespace vca
{

namespace
{

struct Scanner
{

    Scanner(CommandQueue& commands,
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
        thread = std::thread{[this] { scan(); }};
    }

    ~Scanner()
    {
        done = true;
        if (thread.joinable())
        {
            thread.join();
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
                    try
                    {
                        contents.words = file_processor.process(path);
                    }
                    catch (const std::exception& e)
                    {
                        // skip file
                        VCA_EXCEPTION(e) << e.what();
                        continue;
                    }
                    commands.push([this,
                                   path = std::move(path),
                                   contents = std::move(contents)]
                                  { user_db.update_file(path, contents); });
                }
            }
            VCA_INFO << "Scanning finished: " << root_dir
                     << " - Took: " << us_to_s(timer.us()) << " s";
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

    CommandQueue& commands;
    fs::path root_dir;
    UserDb& user_db;
    const FileProcessor& file_processor;
    std::atomic<bool> done{false};
    std::thread thread;
};

} // namespace

struct FileScanner::Impl : public UserConfig::Observer
{
    Impl(CommandQueue& commands,
         UserConfig& user_config,
         UserDb& user_db,
         const FileProcessor& file_processor)
        : commands{commands}
        , user_db{user_db}
        , file_processor{file_processor}
        , user_config{user_config}
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
        // find dirs that don't need scanning anymore
        std::set<fs::path> trash;
        for (const auto& [dir, s] : scanners)
        {
            if (user_config.root_dirs().find(dir) ==
                user_config.root_dirs().end())
            {
                trash.emplace(dir);
            }
        }
        for (const auto& dir : trash)
        {
            scanners.erase(dir);
            user_db.remove_root_dir(dir);
        }
        // add new dirs that need scanning
        for (const auto& dir : user_config.root_dirs())
        {
            if (scanners.find(dir) == scanners.end())
            {
                user_db.add_root_dir(dir);
                try
                {
                    scanners.emplace(
                        dir,
                        std::make_unique<Scanner>(
                            commands, dir, user_db, file_processor));
                }
                catch (...)
                {
                }
            }
        }
    }

    CommandQueue& commands;
    UserDb& user_db;
    const FileProcessor& file_processor;
    UserConfig& user_config;
    std::map<fs::path, std::unique_ptr<Scanner>> scanners;
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
