#pragma once

#include "filesystem.h"
#include "utils.h"

namespace vca
{

class AudioRecorder
{
public:
    explicit AudioRecorder(fs::path path);

    VCA_DELETE_COPY(AudioRecorder)
    VCA_DEFAULT_MOVE(AudioRecorder)

    ~AudioRecorder();

    void
    start();

    void
    stop();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
