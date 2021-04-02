#pragma once

#include "platform.h"

#ifdef VCA_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#ifdef VCA_COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include <nlohmann/json.hpp>
#ifdef VCA_COMPILER_GCC
#pragma GCC diagnostic pop
#endif
#ifdef VCA_COMPILER_CLANG
#pragma clang diagnostic pop
#endif
