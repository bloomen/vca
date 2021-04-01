#include "file_scanner.h"

#include <atomic>
#include <thread>

#include <vca/logging.h>

namespace vca
{

struct FileScanner::Impl
{
    Impl(const AppConfig& app_config,
         const UserConfig& user_config,
         UserDb& user_db,
         const FileProcessor& file_processor)
        : app_config{app_config}
        , user_config{user_config}
        , user_db{user_db}
        , file_processor{file_processor}
        , m_thread{[this] { scan(); }}
    {
    }

    ~Impl()
    {
        m_done = true;
        m_thread.join();
    }

    void
    scan()
    {
        for (const auto& p :
             fs::recursive_directory_iterator{user_config.root_dir()})
        {
            if (m_done)
            {
                break;
            }
            const fs::path path = p;
            if (app_config.matches_ext(path))
            {
                vca::FileContents contents;
                contents.words = file_processor.process(path);
                user_db.update_file(path, contents);
            }
        }
        VCA_INFO << "Scanning finished";
    }

    const AppConfig& app_config;
    const UserConfig& user_config;
    UserDb& user_db;
    const FileProcessor& file_processor;
    std::atomic<bool> m_done{false};
    std::thread m_thread;
};

FileScanner::FileScanner(const AppConfig& app_config,
                         const UserConfig& user_config,
                         UserDb& user_db,
                         const FileProcessor& file_processor)
    : m_impl{std::make_unique<Impl>(app_config,
                                    user_config,
                                    user_db,
                                    file_processor)}
{
}

FileScanner::~FileScanner() = default;

} // namespace vca
