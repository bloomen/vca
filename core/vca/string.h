#pragma once

#include <set>
#include <string>
#include <vector>

#include <tinyutf8/tinyutf8.h>

namespace vca
{

using Char = char32_t;
using String = tiny_utf8::basic_string<Char, char>;

const std::string&
special_chars();

const std::set<Char>&
spec_chars();

void
to_lower_case(std::string& str);

void
to_lower_case(String& str);

bool
is_numeric(const std::string& str);

void
replace_all(std::string& str, const std::string& chars, char with);

void
split(std::vector<std::string>& vec,
      const std::string& str,
      char delimiter = ' ');

} // namespace vca
