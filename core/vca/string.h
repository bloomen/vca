#pragma once

#include <list>
#include <string>
#include <unordered_set>
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
// using String = tiny_utf8::basic_string<Char, char>;
using String = std::basic_string<Char>;

String
narrow_to_wide(const std::string& narrow);

std::string
wide_to_narrow(const String& wide);

const std::string&
special_chars();

const std::unordered_set<Char>&
spec_chars();

void
to_lower_case(std::string& str);

void
to_lower_case(String& str);

void
to_upper_case(String& str);

bool
is_numeric(const std::string& str);

bool
is_numeric(const String& str);

void
replace_all(std::string& str, const std::string& chars, char with);

void
replace_all(String& str, const std::unordered_set<Char>& chars, Char with);

void
split(std::vector<std::string>& vec,
      const std::string& str,
      char delimiter = ' ');

void
split(std::list<String>& vec, const String& str, Char delimiter = U' ');

void
trim(String& str);

} // namespace vca
