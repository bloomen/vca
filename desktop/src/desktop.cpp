#include <iostream>

#include <boost/algorithm/string.hpp>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <vca/config.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>
#include <vca/time.h>
#include <vca/utils.h>

int
main(const int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./vca_desktop <work_dir>" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        const auto work_dir = fs::u8path(argv[1]);

        vca::init_logging();

        vca::SqliteUserDb user_db{work_dir / "user.db",
                                  vca::UserDb::OpenType::ReadOnly};

        auto cmdline = spdlog::stdout_logger_mt("cmdline_logger");
        cmdline->set_formatter(std::make_unique<spdlog::pattern_formatter>(
            "%v", spdlog::pattern_time_type::local, ""));

        std::string input;
        for (;;)
        {
            cmdline->info(">>> ");
            std::getline(std::cin, input);
            if (input.empty())
            {
                continue;
            }

            std::vector<std::string> values;
            boost::split(values, input, boost::is_any_of(" "));

            if (values[0] == "q") // quit
            {
                break;
            }
            else if (values[0] == "s") // search
            {
                if (values.size() <= 1)
                {
                    cmdline->info("Empty search string.\n");
                    continue;
                }
                const vca::FileContents file_contents{
                    {values.begin() + 1, values.end()}};

                cmdline->info("Searching ...\n");
                vca::Timer timer;
                const auto result = user_db.search(file_contents);
                cmdline->info("Search took: {} us\n", timer.us());

                for (const auto& path : result)
                {
                    cmdline->info(path.u8string() + "\n");
                }
            }
            else
            {
                cmdline->info("Unknown command.\n");
            }
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        VCA_EXCEPTION(e) << e.what();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        VCA_ERROR << "Unknown exception";
        return EXIT_FAILURE;
    }
}
