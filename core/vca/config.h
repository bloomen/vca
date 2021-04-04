#pragma once

#include <set>
#include <string>

#include "command_queue.h"
#include "file_lock.h"
#include "filesystem.h"

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
    const std::set<std::string>&
    extensions() const;

    bool
    matches_ext(const fs::path& path) const;

private:
    std::set<std::string> m_extensions = {".txt"};
};

} // namespace vca
