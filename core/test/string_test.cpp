#include <gtest/gtest.h>

#include <iostream>
#include <vca/string.h>

TEST(string, to_lower_case_withEmptyString)
{
    vca::String str;
    vca::to_lower_case(str);
    const vca::String str_exp;
    ASSERT_EQ(str_exp, str);
}

TEST(string, to_lower_case_withGerman)
{
    vca::String str = U"Martin KÜHN";
    vca::to_lower_case(str);
    const vca::String str_exp = U"martin kühn";
    ASSERT_EQ(str_exp, str);
}

TEST(string, to_lower_case_withChinese)
{
    vca::String str = U"你好";
    vca::to_lower_case(str);
    const vca::String str_exp = U"你好";
    ASSERT_EQ(str_exp, str);
}

TEST(string, to_upper_case_withEmptyString)
{
    vca::String str;
    vca::to_upper_case(str);
    const vca::String str_exp;
    ASSERT_EQ(str_exp, str);
}

TEST(string, to_upper_case_withGerman)
{
    vca::String str = U"martin kühn";
    vca::to_upper_case(str);
    const vca::String str_exp = U"MARTIN KÜHN";
    ASSERT_EQ(str_exp, str);
}

TEST(string, to_upper_case_withChinese)
{
    vca::String str = U"你好";
    vca::to_upper_case(str);
    const vca::String str_exp = U"你好";
    ASSERT_EQ(str_exp, str);
}
