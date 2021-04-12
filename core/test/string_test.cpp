#include <gtest/gtest.h>

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

TEST(string, is_numeric)
{
    ASSERT_TRUE(vca::is_numeric(U"0"));
    ASSERT_TRUE(vca::is_numeric(U"123"));
    ASSERT_TRUE(vca::is_numeric(U"123.42"));
    ASSERT_TRUE(vca::is_numeric(U"-123.42"));
    ASSERT_TRUE(vca::is_numeric(U"1e42"));
    ASSERT_TRUE(vca::is_numeric(U"0.6e+42"));
    ASSERT_TRUE(vca::is_numeric(U"-0.3E-42"));
    ASSERT_FALSE(vca::is_numeric(U" -0.3E-42 "));
    ASSERT_FALSE(vca::is_numeric(U"a0"));
    ASSERT_FALSE(vca::is_numeric(U"a"));
    ASSERT_FALSE(vca::is_numeric(U"abc 123"));
}

TEST(string, replace_all)
{
    vca::String str = U"你好, martin kühn!";
    vca::replace_all(str, {U',', U'!'}, U' ');
    const vca::String str_exp = U"你好  martin kühn ";
    ASSERT_EQ(str_exp, str);
}

TEST(string, replace_all_withEmptyChars)
{
    vca::String str = U"你好, martin kühn!";
    vca::replace_all(str, {}, U' ');
    const vca::String str_exp = U"你好, martin kühn!";
    ASSERT_EQ(str_exp, str);
}

TEST(string, replace_all_withEmptyStr)
{
    vca::String str;
    vca::replace_all(str, {U',', U'!'}, U' ');
    const vca::String str_exp;
    ASSERT_EQ(str_exp, str);
}

TEST(string, split)
{
    const vca::String str = U"你好  martin kühn";
    std::list<vca::String> vec;
    vca::split(vec, str);
    std::list<vca::String> vec_exp{U"你好", U"martin", U"kühn"};
    ASSERT_EQ(vec_exp, vec);
}

TEST(string, split_withNoDelims)
{
    const vca::String str = U"你好martinkühn";
    std::list<vca::String> vec;
    vca::split(vec, str);
    std::list<vca::String> vec_exp{U"你好martinkühn"};
    ASSERT_EQ(vec_exp, vec);
}

TEST(string, split_withEmpyStr)
{
    const vca::String str;
    std::list<vca::String> vec;
    vca::split(vec, str);
    std::list<vca::String> vec_exp;
    ASSERT_EQ(vec_exp, vec);
}

TEST(string, trim)
{
    vca::String str = U"  你好, martin kühn! ";
    vca::trim(str);
    const vca::String str_exp = U"  你好, martin kühn! ";
    ASSERT_EQ(str_exp, str);
}

TEST(string, trim_withNothingToDo)
{
    vca::String str = U"你好, martin kühn!";
    vca::trim(str);
    const vca::String str_exp = U"你好, martin kühn!";
    ASSERT_EQ(str_exp, str);
}

TEST(string, trim_withEmptyStr)
{
    vca::String str;
    vca::trim(str);
    const vca::String str_exp;
    ASSERT_EQ(str_exp, str);
}

TEST(string, wide_to_narrow_andBack)
{
    const vca::String str = U"你好, martin kühn!";
    const auto narrow = vca::wide_to_narrow(str);
    const auto wide = vca::narrow_to_wide(narrow);
    ASSERT_EQ(str, wide);
}

TEST(string, wide_to_narrow_andBack_withEmptyInput)
{
    const vca::String str;
    const auto narrow = vca::wide_to_narrow(str);
    const auto wide = vca::narrow_to_wide(narrow);
    ASSERT_EQ(str, wide);
}
