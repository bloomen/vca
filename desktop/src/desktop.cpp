#include <iostream>

#include <boost/algorithm/string.hpp>

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

        std::string input;
        for (;;)
        {
            std::cout << ">>> ";
            std::cin >> input;

            if (input == ".quit")
            {
                break;
            }

            std::vector<std::string> words;
            boost::split(words, input, boost::is_any_of(" "));
            const vca::FileContents file_contents{words};

            std::cout << "Searching ..." << std::endl;
            vca::Timer timer;
            const auto result = user_db.search(file_contents);
            std::cout << "Search took: " << timer.us() << " us" << std::endl;

            for (const auto& path : result)
            {
                std::cout << path << std::endl;
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
