#pragma once

#include <unordered_map>

#include "string.h"

namespace vca
{

const std::unordered_map<Char, Char>&
lower_to_upper_map();

const std::unordered_map<Char, Char>&
upper_to_lower_map();

} // namespace vca
