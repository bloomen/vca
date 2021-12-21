#include "string.h"

#include <algorithm>
#include <codecvt>
#include <map>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include "case_mappings.h"

namespace vca
{

#if defined(VCA_PLATFORM_WINDOWS) && _MSC_VER < 2000

String
narrow_to_wide(const std::string& narrow)
{
    std::wstring_convert<std::codecvt_utf8<__int32>, __int32> cvt;
    auto r = cvt.from_bytes(narrow.data(), narrow.data() + narrow.size());
    String s = reinterpret_cast<const Char*>(r.data());
    return s;
}

std::string
wide_to_narrow(const String& wide)
{
    std::wstring_convert<std::codecvt_utf8<__int32>, __int32> cvt;
    auto p = reinterpret_cast<const int32_t*>(wide.data());
    return cvt.to_bytes(p, p + wide.size());
}

#else

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

#endif

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

void
xml_unescape(String& text)
{
    static const std::map<String, String> str_map{{U"&amp;", U"&"},
                                                  {U"&quot;", U"\""},
                                                  {U"&apos;", U"'"},
                                                  {U"&lt;", U"<"},
                                                  {U"&gt;", U">"}};
    for (const auto& [from, to] : str_map)
    {
        boost::replace_all(text, from, to);
    }
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
    const auto text = m_content.substr(start_index, end_index - start_index);
    auto wide_text = narrow_to_wide(text);
    xml_unescape(wide_text);
    return wide_text;
}

String
xml_tag_content(const std::string& content, const size_t max_byte_count)
{
    XMLParser parser{content};

    String one_line;
    size_t byte_count = 0;
    while (!parser.end() && byte_count < max_byte_count)
    {
        auto line = parser.next();
        trim(line);
        if (line.empty())
        {
            continue;
        }
        byte_count += line.size() * 4;
        if (line.back() == U'-')
        {
            one_line.insert(one_line.end(), line.begin(), line.end() - 1);
        }
        else
        {
            one_line += line + U" ";
        }
    }

    xml_unescape(one_line);
    return one_line;
}

std::vector<String>
tokenize(String line)
{
    replace_all(line, special_chars(), space_char());
    replace_all(line, end_of_line_chars(), space_char());

    std::vector<String> words;
    for (auto& c : line)
    {
        if (is_cjk(c))
        {
            words.emplace_back(String{1, c});
            c = space_char();
        }
    }

    std::list<String> tokens;
    split(tokens, line, space_char());

    for (auto& t : tokens)
    {
        trim(t);
        if (t.size() <= 1)
        {
            continue;
        }
        if (is_numeric(t))
        {
            continue;
        }
        words.emplace_back(std::move(t));
    }

    return words;
}

bool
is_cjk(const Char c)
{
    if (c >= 0x4e00 && c <= 0x9fff) // chinese
    {
        return true;
    }
    if (c >= 0x3040 && c <= 0x30ff) // japanese
    {
        return true;
    }
    if (c >= 0xac00 && c <= 0xd7a3) // korean
    {
        return true;
    }
    return false;
}

} // namespace vca
