#include "string.h"

#include <algorithm>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

namespace vca
{

// TODO: Do these functions work with UTF-8?

const std::string&
special_chars()
{
    static const std::string specials =
        std::string{R"( _-,.?!;:(){}[]~`@#$%^&*+=|\/"'<>)"} + std::string{"\t"};
    return specials;
}

void
to_lower_case(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
}

bool
is_numeric(const std::string& str)
{
    std::string::const_iterator first(str.begin()), last(str.end());
    return boost::spirit::qi::parse(first,
                                    last,
                                    boost::spirit::double_ >>
                                        *boost::spirit::qi::space) &&
        first == last;
}

std::string
remove_all(const std::string& str, const std::string& chars)
{
    const std::set<char> chars_set{chars.begin(), chars.end()};
    std::vector<char> result;
    for (const auto v : str)
    {
        if (chars_set.count(v) == 0)
        {
            result.emplace_back(v);
        }
    }
    return {result.begin(), result.end()};
}

void
replace_all(std::string& str, const std::string& chars, const char with)
{
    const std::set<char> chars_set{chars.begin(), chars.end()};
    for (char& c : str)
    {
        if (chars_set.count(c) > 0)
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
    boost::split(vec, str, boost::is_any_of(std::string{1, delimiter}));
}

std::string
merge_consecutive(const std::string& str, const char c)
{
    bool merging = false;
    std::vector<char> result;
    for (const auto v : str)
    {
        if (v == c)
        {
            if (!merging)
            {
                result.emplace_back(v);
                merging = true;
            }
        }
        else
        {
            merging = false;
            result.emplace_back(v);
        }
    }
    return {result.begin(), result.end()};
}

} // namespace vca
