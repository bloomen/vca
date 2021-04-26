#pragma once

#include <list>
#include <string>
#include <unordered_set>
#include <vector>

#include "platform.h"

namespace vca
{

using Char = char32_t;
using String = std::basic_string<Char>;

String
narrow_to_wide(const std::string& narrow);

std::string
wide_to_narrow(const String& wide);

const std::unordered_set<Char>&
special_chars();

const std::unordered_set<Char>&
end_of_line_chars();

Char
line_feed_char();

Char
space_char();

void
to_lower_case(String& str);

void
to_upper_case(String& str);

bool
is_numeric(const String& str);

void
replace_all(String& str, const std::unordered_set<Char>& chars, Char with);

void
split(std::list<String>& vec, const String& str, Char delimiter);

void
trim(String& str);

void
xml_unescape(String& text);

class XMLParser
{
public:
    explicit XMLParser(const std::string& content);

    bool
    end() const;

    String
    next();

private:
    const std::string& m_content;
    int m_index = 0;
};

std::vector<String>
tokenize(String line);

} // namespace vca
