#pragma once

#include <set>
#include <string>

#include "command_queue.h"
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

    std::string
    as_json() const;

    void
    set_json(const std::string& json);

    const std::set<fs::path>&
    root_dirs() const;

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

    const std::string&
    host() const;
    void
    set_port(int port);
    int
    port() const;
    std::string
    url() const;

    void
    write(const fs::path& filename) const;
    static AppConfig
    read(const fs::path& filename);

private:
    std::string m_host{"127.0.0.1"};
    int m_port{7777};
};

} // namespace vca
