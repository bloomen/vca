#include <iostream>

#include <boost/algorithm/string.hpp>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <vca/config.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>
#include <vca/string.h>
#include <vca/time.h>
#include <vca/utils.h>

int
main(const int, char**)
{
    try
    {
        const auto work_dir = vca::user_config_dir() / "vca";
        fs::create_directories(work_dir);

        vca::init_logging();

        auto cmdline = spdlog::stdout_logger_mt("cmdline_logger");
        cmdline->set_formatter(std::make_unique<spdlog::pattern_formatter>(
            "%v", spdlog::pattern_time_type::local, ""));

        vca::SqliteUserDb user_db{work_dir / "user_data" / "user.db",
                                  vca::UserDb::OpenType::ReadOnly};

        std::string input;
        for (;;)
        {
            cmdline->info(">>> ");
            std::getline(std::cin, input);
            if (input.empty())
            {
                continue;
            }
            auto wide_input = vca::narrow_to_wide(input);
            vca::trim(wide_input);

            std::list<vca::String> values;
            vca::split(values, wide_input, vca::space_char());

            if (values.front() == U"q") // quit
            {
                break;
            }
            else if (values.front() == U"s") // search
            {
                values.erase(values.begin());
                if (values.empty())
                {
                    cmdline->info("Empty search string.\n");
                    continue;
                }

                const auto file_contents =
                    vca::FileContents::fromSearch(values);

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
