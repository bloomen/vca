#include <iostream>

#include <boost/algorithm/string.hpp>
#include <restclient-cpp/restclient.h>
#include <served/served.hpp>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include <vca/config.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/string.h>
#include <vca/time.h>
#include <vca/utils.h>
#include <vca/json.h>

using json = nlohmann::json;

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
                std::string value;
                for (const auto& v : values)
                {
                    value += vca::wide_to_narrow(v);
                    value += " ";
                }

                VCA_INFO << "Searching ...";
                vca::Timer timer;
                const auto query = "http://localhost:7777/s?v=" + served::query_escape(value);
                VCA_INFO << "Query: " << query;
                const auto response = RestClient::get(query);
                if (response.code != 200)
                {
                    VCA_ERROR << "Received error code: " << response.code;
                    continue;
                }
                VCA_INFO << "Search took: " << timer.us() << " us";
                json j = json::parse(response.body);

                for (const auto& result : j["results"])
                {
                    cmdline->info((fs::u8path(result["d"].get<std::string>()) / fs::u8path(result["f"].get<std::string>())).u8string() + "\n");
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
