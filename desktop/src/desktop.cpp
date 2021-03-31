#include <csignal>
#include <iostream>

#include <vca/config.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>
#include <vca/utils.h>

void
sigint_handler(int)
{
    VCA_INFO << "Caught SIGINT";
    VCA_INFO << "Terminating desktop";
}

int
main(int argc, char** argv)
{
    std::signal(SIGINT, sigint_handler);

    if (argc != 2)
    {
        std::cerr << "Usage: ./vca_desktop <work_dir>" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        const auto work_dir = fs::u8path(argv[1]);

        vca::SqliteUserDb user_db{work_dir / "user.db",
                                  vca::UserDb::OpenType::read_only};

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        VCA_ERROR << "Exception: " << e.what();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        VCA_ERROR << "Unknown exception";
        return EXIT_FAILURE;
    }
}
