#pragma once

#include <set>
#include <string>
#include <vector>

#include "platform.h"

#ifdef VCA_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#include <tinyutf8/tinyutf8.h>
#ifdef VCA_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

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

void
to_upper_case(String& str);

bool
is_numeric(const std::string& str);

void
replace_all(std::string& str, const std::string& chars, char with);

void
split(std::vector<std::string>& vec,
      const std::string& str,
      char delimiter = ' ');

} // namespace vca
