#pragma once

#include <memory>

#include <vca/command_queue.h>
#include <vca/config.h>
#include <vca/userdb.h>
#include <vca/utils.h>

#include "file_processor.h"

namespace vca
{

class FileWatcher
{
public:
    FileWatcher(CommandQueue& commands,
                UserConfig& user_config,
                UserDb& user_db,
                const FileProcessor& file_processor);

    VCA_DELETE_COPY(FileWatcher)
    VCA_DEFAULT_MOVE(FileWatcher)

    ~FileWatcher();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
