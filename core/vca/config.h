#pragma once

#include <set>
#include <string>

#include "command_queue.h"
#include "file_lock.h"
#include "filesystem.h"
#include "string.h"

namespace vca
{

class UserConfig
{
public:
    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void
        user_config_changed(const UserConfig& user_config) = 0;
    };

    explicit UserConfig(CommandQueue& commands, fs::path path);

    VCA_DELETE_COPY(UserConfig)
    VCA_DEFAULT_MOVE(UserConfig)

    ~UserConfig();

    const fs::path&
    root_dir() const;

    void
    add_observer(Observer& observer);

    void
    remove_observer(Observer& observer);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

class AppConfig
{
public:
    AppConfig() = default;

    VCA_DELETE_COPY(AppConfig)
    VCA_DEFAULT_MOVE(AppConfig)

    const std::set<String>&
    extensions() const;

private:
    std::set<String> m_extensions = {U".txt"};
};

} // namespace vca
