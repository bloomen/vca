#pragma once

#include <string>
#include <vector>

namespace vca
{

const std::string&
special_chars();

void
to_lower_case(std::string& str);

bool
is_numeric(const std::string& str);

void
replace_all(std::string& str, const std::string& chars, char with);

void
split(std::vector<std::string>& vec,
      const std::string& str,
      char delimiter = ' ');

} // namespace vca
