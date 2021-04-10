#include <iostream>

#include <boost/algorithm/string.hpp>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <vca/audio.h>
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

        vca::AudioRecorder audio_recorder{work_dir / "audio.raw"};

        std::string input;
        for (;;)
        {
            cmdline->info(">>> ");
            std::getline(std::cin, input);
            if (input.empty())
            {
                continue;
            }
            boost::trim(input);

            std::vector<std::string> values;
            vca::split(values, input);

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
                for (auto& v : values)
                {
                    boost::trim(v);
                }
                values.erase(
                    std::remove_if(values.begin(),
                                   values.end(),
                                   [](const auto& v) { return v.empty(); }),
                    values.end());
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
            else if (values[0] == "r") // record
            {
                if (values.size() <= 1)
                {
                    cmdline->info("Empty record command.\n");
                    continue;
                }
                if (values[1] == "start")
                {
                    audio_recorder.start();
                }
                else if (values[1] == "stop")
                {
                    audio_recorder.stop();
                }
                else
                {
                    cmdline->info("Unknown record command.\n");
                    continue;
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
