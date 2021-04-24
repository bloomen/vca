#include "string.h"

#include <algorithm>
#include <codecvt>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include "case_mappings.h"

namespace vca
{

namespace
{

void
xml_unescape(std::string& text)
{
    static const std::map<std::string, std::string> str_map{{"&amp;", "&"},
                                                            {"&quot;", "\""},
                                                            {"&apos;", "'"},
                                                            {"&lt;", "<"},
                                                            {"&gt;", ">"}};
    for (const auto& [from, to] : str_map)
    {
        boost::replace_all(text, from, to);
    }
}

} // namespace

String
narrow_to_wide(const std::string& narrow)
{
    std::wstring_convert<std::codecvt_utf8<Char>, Char> cvt;
    return cvt.from_bytes(narrow);
}

std::string
wide_to_narrow(const String& wide)
{
    std::wstring_convert<std::codecvt_utf8<Char>, Char> cvt;
    return cvt.to_bytes(wide);
}

const std::unordered_set<Char>&
special_chars()
{
    static const std::unordered_set<Char> specials{
        U'\x3001', U'\xff1f', U'\xff09', U'\x2026', U'\xff08', U'\x3002',
        U'\xff0c', U'\x9',    U'\x3e',   U'\x3c',   U'\x2019', U'\x27',
        U'\x22',   U'\x7c',   U'\x3d',   U'\x201d', U'\x2b',   U'\x201c',
        U'\xb7',   U'\x2a',   U'\x2018', U'\x26',   U'\x2f',   U'\x5e',
        U'\x25',   U'\x24',   U'\x23',   U'\x40',   U'\x60',   U'\x300b',
        U'\x7e',   U'\x300a', U'\x7d',   U'\x7b',   U'\x29',   U'\x28',
        U'\x3a',   U'\x3b',   U'\x21',   U'\x3f',   U'\x5d',   U'\x2e',
        U'\x5b',   U'\x2c',   U'\x5c',   U'\x2d',   U'\x5f'};
    return specials;
}

const std::unordered_set<Char>&
end_of_line_chars()
{
    static const std::unordered_set<Char> eols{
        U'\xa', U'\xb', U'\xc', U'\xd', U'\x85', U'\x2028', U'\x2029'};
    return eols;
}

Char
line_feed_char()
{
    return U'\xa';
}

Char
space_char()
{
    return U'\x20';
}

void
to_lower_case(String& str)
{
    const auto& u2l = upper_to_lower_map();
    std::transform(str.begin(), str.end(), str.begin(), [&u2l](const Char c) {
        const auto it = u2l.find(c);
        if (it != u2l.end())
        {
            return it->second;
        }
        return c;
    });
}

void
to_upper_case(String& str)
{
    const auto& l2u = lower_to_upper_map();
    std::transform(str.begin(), str.end(), str.begin(), [&l2u](const Char c) {
        const auto it = l2u.find(c);
        if (it != l2u.end())
        {
            return it->second;
        }
        return c;
    });
}

bool
is_numeric(const String& str)
{
    typename String::const_iterator first(str.begin()), last(str.end());
    return boost::spirit::qi::parse(first,
                                    last,
                                    boost::spirit::double_ >>
                                        *boost::spirit::qi::space) &&
        first == last;
}

void
replace_all(String& str, const std::unordered_set<Char>& chars, Char with)
{
    for (auto& c : str)
    {
        if (chars.count(c) > 0)
        {
            c = with;
        }
    }
}

void
split(std::list<String>& vec, const String& str, const Char delimiter)
{
    const String delim{1, delimiter};
    boost::split(vec, str, boost::is_any_of(delim));
    vec.remove_if([](const auto& s) { return s.empty(); });
}

void
trim(String& str)
{
    boost::algorithm::trim_if(
        str,
        boost::is_any_of(U"\x2000\x2001\x2002\x2003\x2004\x2005\x2006\x2007"
                         U"\x2009\x200A\x2028\x2029\x202f\x205f\x3000"));
}

XMLParser::XMLParser(const std::string& content)
    : m_content{content}
{
}

bool
XMLParser::end() const
{
    return static_cast<size_t>(m_index + 1) >= m_content.size();
}

String
XMLParser::next()
{
    constexpr char start_tag = '>';
    constexpr char end_tag = '<';
    int start_index = -1;
    int end_index = -1;
    while (!end())
    {
        if (m_content[m_index] == start_tag)
        {
            start_index = m_index + 1;
        }
        else if (start_index >= 0 && m_content[m_index] == end_tag)
        {
            end_index = m_index - 1;
            break;
        }
        ++m_index;
    }
    if (start_index >= end_index)
    {
        return {};
    }
    auto text = m_content.substr(start_index, end_index - start_index);
    xml_unescape(text);
    return narrow_to_wide(text);
}

} // namespace vca
