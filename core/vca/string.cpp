#include "string.h"

#include <algorithm>
#include <codecvt>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include "case_mappings.h"

namespace vca
{

// TODO: Do these functions work with UTF-8?

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

const std::string&
special_chars()
{
    static const std::string specials =
        std::string{R"( _-,.?!;:(){}[]~`@#$%^&*+=|\/"'<>)"} + "\t" + "。" +
        "，" + "。" + "·" + "《" + "》" + "？" + "“" + "”" + "、" + "‘" + "’";
    return specials;
}

const std::unordered_set<Char>&
spec_chars()
{
    static const std::unordered_set<Char> specials{
        U'_',  U'-',  U',',  U'.', U'?', U'!',  U';',  U':',  U'(',
        U')',  U'{',  U'}',  U'[', U']', U'~',  U'`',  U'@',  U'#',
        U'$',  U'%',  U'^',  U'&', U'*', U'+',  U'=',  U'|',  U'\\',
        U'/',  U'"',  U'\'', U'<', U'>', U'\t', U'。', U'，', U'·',
        U'《', U'》', U'？', U'“', U'”', U'、', U'‘',  U'’'};
    return specials;
}

void
to_lower_case(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
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
is_numeric(const std::string& str)
{
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
replace_all(std::string& str, const std::string& chars, const char with)
{
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
split(std::vector<std::string>& vec,
      const std::string& str,
      const char delimiter)
{
    const std::string delim{1, delimiter};
    boost::split(vec, str, boost::is_any_of(delim));
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

} // namespace vca
