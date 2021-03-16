#pragma once

namespace vca
{

const auto ignore_op = [](auto&&...) {};

} // namespace vca

#define VCA_NODISCARD [[nodiscard]]

#define VCA_DELETE_COPY(class_name)                                            \
    class_name(const class_name&) = delete;                                    \
    class_name& operator=(const class_name&) = delete;

#define VCA_DELETE_MOVE(class_name)                                            \
    class_name(class_name&&) = delete;                                         \
    class_name& operator=(class_name&&) = delete;

#define VCA_DEFAULT_COPY(class_name)                                           \
    class_name(const class_name&) = default;                                   \
    class_name& operator=(const class_name&) = default;

#define VCA_DEFAULT_MOVE(class_name)                                           \
    class_name(class_name&&) = default;                                        \
    class_name& operator=(class_name&&) = default;
