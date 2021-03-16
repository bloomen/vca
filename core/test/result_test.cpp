#include <gtest/gtest.h>

#include <vca/result.h>

namespace
{

vca::Result<double>
success_func()
{
    return 13.3;
}

vca::Result<void>
success_func_void()
{
    return {};
}

vca::Result<double>
fail_func()
{
    return vca::Error{"rough day"};
}

vca::Result<void>
fail_func_void()
{
    return vca::Error{"rough day again"};
}

} // namespace

TEST(result, result_basics)
{
    auto res1 = success_func();
    ASSERT_TRUE(res1.ok());
    ASSERT_EQ(13.3, res1.value());
    auto res2 = fail_func();
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ("rough day", res2.error().message());
}

TEST(result, result_unwrap)
{
    auto data = success_func().unwrap();
    ASSERT_EQ(13.3, data);
    auto res = success_func();
    auto d = res.unwrap();
    ASSERT_EQ(13.3, d);
    ASSERT_EQ(13.3, res.unwrap());
    ASSERT_THROW(fail_func().unwrap(), vca::ResultError);
    auto res2 = fail_func();
    ASSERT_THROW(res2.unwrap(), vca::ResultError);
}

TEST(result, result_match_success)
{
    success_func().match([](double x) { EXPECT_EQ(13.3, x); },
                         [](auto) { EXPECT_TRUE(false); });
    auto res = success_func();
    res.match([](double x) { EXPECT_EQ(13.3, x); },
              [](auto) { EXPECT_TRUE(false); });
    res.match([](double x) { EXPECT_EQ(13.3, x); },
              [](auto) { EXPECT_TRUE(false); });
    std::move(res).match([](double x) { EXPECT_EQ(13.3, x); },
                         [](auto) { EXPECT_TRUE(false); });
}

TEST(result, result_match_error)
{
    fail_func().match([](double) { EXPECT_TRUE(false); },
                      [](auto e) { EXPECT_EQ("rough day", e.message()); });
    auto res = fail_func();
    res.match([](double) { EXPECT_TRUE(false); },
              [](auto e) { EXPECT_EQ("rough day", e.message()); });
}

TEST(result, result_void)
{
    auto res1 = success_func_void();
    ASSERT_TRUE(res1.ok());
    auto res2 = fail_func_void();
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ("rough day again", res2.error().message());
}

TEST(result, result_void_unwrap)
{
    success_func().unwrap();
    ASSERT_THROW(fail_func().unwrap(), vca::ResultError);
}

TEST(result, result_void_match_success)
{
    success_func().match([](double) { EXPECT_TRUE(true); },
                         [](auto) { EXPECT_TRUE(false); });
}

TEST(result, result_void_match_error)
{
    fail_func().match([](double) { EXPECT_TRUE(false); },
                      [](auto) { EXPECT_TRUE(true); });
}
