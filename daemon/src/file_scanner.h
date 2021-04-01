#pragma once

#include <memory>

#include <vca/config.h>
#include <vca/userdb.h>
#include <vca/utils.h>

#include "file_processor.h"

namespace vca
{

class FileScanner
{
public:
    FileScanner(const AppConfig& app_config,
                const UserConfig& user_config,
                UserDb& user_db,
                const FileProcessor& file_processor);

    VCA_DELETE_COPY(FileScanner)
    VCA_DEFAULT_MOVE(FileScanner)

    ~FileScanner();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
