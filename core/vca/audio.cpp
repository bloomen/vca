#include "audio.h"

#include <portaudio.h>

namespace vca
{

struct AudioRecorder::Impl
{
    Impl(fs::path path)
        : path{std::move(path)}
    {
    }

    fs::path path;
};

AudioRecorder::AudioRecorder(fs::path path)
    : m_impl{std::make_unique<Impl>(std::move(path))}
{
}

AudioRecorder::~AudioRecorder() = default;

void
AudioRecorder::start()
{
}

void
AudioRecorder::stop()
{
}

} // namespace vca
