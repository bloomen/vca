#pragma once

#include <chrono>

namespace vca
{

class Timer
{
public:
    using ClockType = std::chrono::high_resolution_clock;

    explicit Timer() = default;

    size_t
    us() const
    {
        using std::chrono::duration;
        const auto stop = ClockType::now();
        return static_cast<size_t>(
            duration<double, std::micro>{stop - m_start}.count());
    }

private:
    ClockType::time_point m_start = ClockType::now();
};

} // namespace vca
