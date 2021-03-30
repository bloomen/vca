#pragma once

#include <memory>

#include <vca/config.h>
#include <vca/userdb.h>
#include <vca/utils.h>

namespace vca
{

class FileWatcher
{
public:
    FileWatcher(const AppConfig& app_config,
                const UserConfig& user_config,
                UserDb& user_db);

    VCA_DELETE_COPY(FileWatcher)
    VCA_DEFAULT_MOVE(FileWatcher)

    ~FileWatcher();

    void
    stop();

    void
    run();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
