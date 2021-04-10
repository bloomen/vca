#pragma once

#include <string>
#include <vector>

namespace vca
{

void
to_lower_case(std::string& str);

bool
is_numeric(const std::string& str);

std::string
remove_all(const std::string& str, const std::string& chars);

void
replace_all(std::string& str, const std::string& chars, char with);

void
split(std::vector<std::string>& vec,
      const std::string& str,
      char delimiter = ' ');

std::string
merge_consecutive(const std::string& str, char c);

} // namespace vca
