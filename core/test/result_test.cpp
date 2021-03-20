#include <gtest/gtest.h>

#include <vca/result.h>

using vca::CustomErr;
using vca::Error;
using vca::Result;

namespace
{

Result<double>
success_func()
{
    return 13.3;
}

Result<void>
success_func_void()
{
    return {};
}

Result<double, CustomErr<int>>
custom_success_func()
{
    return 13.3;
}

Result<void, CustomErr<int>>
custom_success_func_void()
{
    return {};
}

Result<double>
fail_func()
{
    return Error{"rough day"};
}

Result<void>
fail_func_void()
{
    return Error{"rough day again"};
}

Result<double, CustomErr<int>>
custom_fail_func()
{
    return CustomErr{1};
}

Result<void, CustomErr<int>>
custom_fail_func_void()
{
    return CustomErr{2};
}

} // namespace

TEST(result, Result_Error_defaultConstructor)
{
    Error error;
    ASSERT_TRUE(error.message().empty());
    ASSERT_EQ(Error::CODE_UNSET, error.code());
    ASSERT_EQ("Error", error.repr());
}

TEST(result, Result_Error_constructedFromLValueString)
{
    const std::string msg = "msg";
    Error error{msg};
    ASSERT_EQ(msg, error.message());
    ASSERT_EQ(Error::CODE_UNSET, error.code());
    ASSERT_EQ("Error: msg", error.repr());
}

TEST(result, Result_Error_constructedFromLValueStringAndCode)
{
    const std::string msg = "msg";
    Error error{42, msg};
    ASSERT_EQ(msg, error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_constructedFromRValueString)
{
    std::string msg = "msg";
    Error error{std::move(msg)};
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(Error::CODE_UNSET, error.code());
    ASSERT_EQ("Error: msg", error.repr());
}

TEST(result, Result_Error_constructedFromRValueStringAndCode)
{
    std::string msg = "msg";
    Error error{42, std::move(msg)};
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_constructedFromCString)
{
    Error error{"msg"};
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(Error::CODE_UNSET, error.code());
    ASSERT_EQ("Error: msg", error.repr());
}

TEST(result, Result_Error_constructedFromCStringAndCode)
{
    Error error{42, "msg"};
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_constructedFromCode)
{
    Error error{42};
    ASSERT_EQ("", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42]", error.repr());
}

TEST(result, Result_Error_copyConstructor)
{
    const Error error0{42, "msg"};
    Error error{error0};
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_copyAssignment)
{
    const Error error0{42, "msg"};
    Error error{43, "msg2"};
    error = error0;
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_moveConstructor)
{
    Error error0{42, "msg"};
    Error error{std::move(error0)};
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_moveAssignment)
{
    Error error0{42, "msg"};
    Error error{43, "msg2"};
    error = std::move(error0);
    ASSERT_EQ("msg", error.message());
    ASSERT_EQ(42, error.code());
    ASSERT_EQ("Error: [42] msg", error.repr());
}

TEST(result, Result_Error_outputStreamOperator_fromLValue)
{
    Error error{42, "msg"};
    std::ostringstream os;
    os << error;
    ASSERT_EQ("Error: [42] msg", os.str());
}

TEST(result, Result_Error_outputStreamOperator_fromRValue)
{
    std::ostringstream os;
    os << Error{42, "msg"};
    ASSERT_EQ("Error: [42] msg", os.str());
}

TEST(result, Result_Error_appendToMessage_fromLValue)
{
    Error e{42, "msg"};
    e << " foo " << 43;
    ASSERT_EQ("Error: [42] msg foo 43", e.repr());
}

TEST(result, Result_Error_appendToMessage_fromRValue)
{
    const auto e = Error{42, "msg"} << " foo " << 43;
    ASSERT_EQ("Error: [42] msg foo 43", e.repr());
}

TEST(result, Result_CustomErr_constructFromLValue)
{
    const auto value = 42;
    CustomErr<int> e1{value};
    ASSERT_EQ(42, e1.get());
    const CustomErr<int> e2{value};
    ASSERT_EQ(42, e2.get());
}

TEST(result, Result_CustomErr_constructFromRValue)
{
    CustomErr<int> e1{42};
    ASSERT_EQ(42, e1.get());
    const CustomErr<int> e2{42};
    ASSERT_EQ(42, e2.get());
}

TEST(result, Result_copyConstructor)
{
    const Result<int> res1 = 42;
    const Result<int> res2{res1};
    ASSERT_TRUE(res1.ok());
    ASSERT_TRUE(res2.ok());
    ASSERT_FALSE(res1.bad());
    ASSERT_FALSE(res2.bad());
    ASSERT_EQ(res1.value_view(), res2.value_view());
    const Result<int> res3 = Error{-1};
    const Result<int> res4{res3};
    ASSERT_TRUE(!res3.ok());
    ASSERT_TRUE(!res4.ok());
    ASSERT_TRUE(res3.bad());
    ASSERT_TRUE(res4.bad());
    ASSERT_EQ(res3.error_view().repr(), res4.error_view().repr());
}

TEST(result, Result_copyAssignment_toSame)
{
    const Result<int> res1 = 42;
    Result<int> res2 = 43;
    res2 = res1;
    ASSERT_TRUE(res1.ok());
    ASSERT_TRUE(res2.ok());
    ASSERT_EQ(res1.value_view(), res2.value_view());
    const Result<int> res3 = Error{-1};
    Result<int> res4 = Error{-2};
    res4 = res3;
    ASSERT_TRUE(!res3.ok());
    ASSERT_TRUE(!res4.ok());
    ASSERT_EQ(res3.error_view().repr(), res4.error_view().repr());
}

TEST(result, Result_copyAssignment_toDifferent)
{
    const Result<int> res1 = 42;
    Result<int> res2 = Error{-1};
    res2 = res1;
    ASSERT_TRUE(res1.ok());
    ASSERT_TRUE(res2.ok());
    ASSERT_EQ(res1.value_view(), res2.value_view());
    const Result<int> res3 = Error{-1};
    Result<int> res4 = 42;
    res4 = res3;
    ASSERT_TRUE(!res3.ok());
    ASSERT_TRUE(!res4.ok());
    ASSERT_EQ(res3.error_view().repr(), res4.error_view().repr());
}

TEST(result, Result_moveConstructor)
{
    Result<int> res1 = 42;
    const Result<int> res2{std::move(res1)};
    ASSERT_TRUE(res2.ok());
    ASSERT_EQ(42, res2.value_view());
    Result<int> res3 = Error{-1};
    const Result<int> res4{std::move(res3)};
    ASSERT_TRUE(!res4.ok());
    ASSERT_EQ(-1, res4.error_view().code());
}

TEST(result, Result_moveAssignment_toSame)
{
    Result<int> res1 = 42;
    Result<int> res2 = 43;
    res2 = std::move(res1);
    ASSERT_TRUE(res2.ok());
    ASSERT_EQ(42, res2.value());
    Result<int> res3 = Error{-1};
    Result<int> res4 = Error{-2};
    res4 = std::move(res3);
    ASSERT_TRUE(!res4.ok());
    ASSERT_EQ(-1, res4.error().code());
}

TEST(result, Result_moveAssignment_toDifferent)
{
    Result<int> res1 = 42;
    Result<int> res2 = Error{-1};
    res2 = std::move(res1);
    ASSERT_TRUE(res2.ok());
    ASSERT_EQ(42, res2.value());
    Result<int> res3 = Error{-1};
    Result<int> res4 = 42;
    res4 = std::move(res3);
    ASSERT_TRUE(!res4.ok());
    ASSERT_EQ(-1, res4.error().code());
}

TEST(result, Result_value_error)
{
    auto res1 = success_func();
    ASSERT_TRUE(res1.ok());
    ASSERT_EQ(13.3, res1.value());
    auto res2 = fail_func();
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ("rough day", res2.error().message());
}

TEST(result, Result_value_error_custom)
{
    auto res1 = custom_success_func();
    ASSERT_TRUE(res1.ok());
    ASSERT_EQ(13.3, res1.value());
    auto res2 = custom_fail_func();
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ(1, res2.error().get());
}

TEST(result, Result_value_error_customWithNonTrivialType)
{
    Result<int, CustomErr<std::string>> res1{42};
    ASSERT_TRUE(res1.ok());
    ASSERT_EQ(42, res1.value());
    Result<int, CustomErr<std::string>> res2{CustomErr<std::string>{"fail"}};
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ("fail", res2.error().get());
}

TEST(result, Result_unwrap)
{
    auto data = success_func().unwrap();
    ASSERT_EQ(13.3, data);
    auto res = success_func();
    auto d = res.unwrap_view();
    ASSERT_EQ(13.3, d);
    ASSERT_EQ(13.3, res.unwrap());
    ASSERT_THROW(fail_func().unwrap(), vca::ResultError);
    auto res2 = fail_func();
    ASSERT_THROW(res2.unwrap_view(), vca::ResultError);
}

TEST(result, Result_unwrap_custom)
{
    auto data = custom_success_func().unwrap();
    ASSERT_EQ(13.3, data);
    auto res = custom_success_func();
    auto d = res.unwrap_view();
    ASSERT_EQ(13.3, d);
    ASSERT_EQ(13.3, res.unwrap());
    ASSERT_THROW(custom_fail_func().unwrap(), vca::ResultError);
    auto res2 = custom_fail_func();
    ASSERT_THROW(res2.unwrap_view(), vca::ResultError);
}

TEST(result, Result_match_success)
{
    int success_calls = 0;
    success_func().match(
        [&success_calls](double&& x) {
            ++success_calls;
            ASSERT_EQ(13.3, x);
        },
        [](Error&&) { FAIL(); });
    auto res = success_func();
    res.match_view(
        [&success_calls](const double& x) {
            ++success_calls;
            ASSERT_EQ(13.3, x);
        },
        [](const Error&) { FAIL(); });
    res.match(
        [&success_calls](double&& x) {
            ++success_calls;
            ASSERT_EQ(13.3, x);
        },
        [](Error&&) { FAIL(); });
    ASSERT_EQ(3, success_calls);
}

TEST(result, Result_match_success_custom)
{
    int success_calls = 0;
    custom_success_func().match(
        [&success_calls](double&& x) {
            ++success_calls;
            ASSERT_EQ(13.3, x);
        },
        [](auto&&) { FAIL(); });
    auto res = custom_success_func();
    res.match_view(
        [&success_calls](const double& x) {
            ++success_calls;
            ASSERT_EQ(13.3, x);
        },
        [](const auto&) { FAIL(); });
    res.match(
        [&success_calls](double&& x) {
            ++success_calls;
            ASSERT_EQ(13.3, x);
        },
        [](auto&&) { FAIL(); });
    ASSERT_EQ(3, success_calls);
}

TEST(result, Result_match_error)
{
    int fail_calls = 0;
    fail_func().match([](double&&) { FAIL(); },
                      [&fail_calls](Error&& e) {
                          ++fail_calls;
                          ASSERT_EQ("rough day", e.message());
                      });
    auto res = fail_func();
    res.match_view([](const double&) { FAIL(); },
                   [&fail_calls](const Error& e) {
                       ++fail_calls;
                       ASSERT_EQ("rough day", e.message());
                   });
    res.match([](const double&) { FAIL(); },
              [&fail_calls](Error&& e) {
                  ++fail_calls;
                  ASSERT_EQ("rough day", e.message());
              });
    ASSERT_EQ(3, fail_calls);
}

TEST(result, Result_match_error_custom)
{
    int fail_calls = 0;
    custom_fail_func().match([](double&&) { FAIL(); },
                             [&fail_calls](auto&& e) {
                                 ++fail_calls;
                                 ASSERT_EQ(1, e.get());
                             });
    auto res = custom_fail_func();
    res.match_view([](const double&) { FAIL(); },
                   [&fail_calls](const auto& e) {
                       ++fail_calls;
                       ASSERT_EQ(1, e.get());
                   });
    res.match([](const double&) { FAIL(); },
              [&fail_calls](auto&& e) {
                  ++fail_calls;
                  ASSERT_EQ(1, e.get());
              });
    ASSERT_EQ(3, fail_calls);
}

TEST(result, Result_void_value_error)
{
    auto res1 = success_func_void();
    ASSERT_TRUE(res1.ok());
    auto res2 = fail_func_void();
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ("rough day again", res2.error().message());
}

TEST(result, Result_void_value_error_custom)
{
    auto res1 = custom_success_func_void();
    ASSERT_TRUE(res1.ok());
    auto res2 = custom_fail_func_void();
    ASSERT_TRUE(!res2.ok());
    ASSERT_EQ(2, res2.error().get());
}

TEST(result, Result_void_unwrap)
{
    success_func_void().unwrap();
    ASSERT_THROW(fail_func_void().unwrap(), vca::ResultError);
}

TEST(result, Result_void_unwrap_custom)
{
    custom_success_func_void().unwrap();
    ASSERT_THROW(custom_fail_func_void().unwrap(), vca::ResultError);
}

TEST(result, Result_void_match_success)
{
    int success_calls = 0;
    success_func_void().match([&success_calls]() { ++success_calls; },
                              [](Error&&) { FAIL(); });
    auto res = success_func_void();
    res.match_view([&success_calls]() { ++success_calls; },
                   [](const Error&) { FAIL(); });
    res.match([&success_calls]() { ++success_calls; }, [](Error&&) { FAIL(); });
    ASSERT_EQ(3, success_calls);
}

TEST(result, Result_void_match_success_custom)
{
    int success_calls = 0;
    custom_success_func_void().match([&success_calls]() { ++success_calls; },
                                     [](auto&&) { FAIL(); });
    auto res = custom_success_func_void();
    res.match_view([&success_calls]() { ++success_calls; },
                   [](const auto&) { FAIL(); });
    res.match([&success_calls]() { ++success_calls; }, [](auto&&) { FAIL(); });
    ASSERT_EQ(3, success_calls);
}

TEST(result, Result_void_match_error)
{
    int fail_calls = 0;
    fail_func_void().match([]() { FAIL(); },
                           [&fail_calls](Error&&) { ++fail_calls; });
    auto res = fail_func_void();
    res.match_view([]() { FAIL(); },
                   [&fail_calls](const Error&) { ++fail_calls; });
    res.match([]() { FAIL(); }, [&fail_calls](Error&&) { ++fail_calls; });
    ASSERT_EQ(3, fail_calls);
}

TEST(result, Result_void_match_error_custom)
{
    int fail_calls = 0;
    custom_fail_func_void().match([]() { FAIL(); },
                                  [&fail_calls](auto&&) { ++fail_calls; });
    auto res = custom_fail_func_void();
    res.match_view([]() { FAIL(); },
                   [&fail_calls](const auto&) { ++fail_calls; });
    res.match([]() { FAIL(); }, [&fail_calls](auto&&) { ++fail_calls; });
    ASSERT_EQ(3, fail_calls);
}

namespace
{

Result<double>
take_result_success()
{
    TAKE_VALUE(auto val, success_func());
    return val;
}

Result<double>
take_result_fail()
{
    TAKE_VALUE(auto val, fail_func());
    return val;
}

} // namespace

TEST(result, Result_TakeResult_success)
{
    auto res = take_result_success();
    ASSERT_TRUE(res.ok());
    ASSERT_EQ(13.3, res.value());
}

TEST(result, Result_TakeResult_fail)
{
    auto res = take_result_fail();
    ASSERT_TRUE(!res.ok());
    ASSERT_EQ("rough day", res.error().message());
}

namespace
{

Result<double>
take_error_success()
{
    CHECK_RESULT(success_func_void());
    return 42;
}

Result<double>
take_error_fail()
{
    CHECK_RESULT(fail_func_void());
    return 42;
}

} // namespace

TEST(result, Result_TakeError_success)
{
    auto res = take_error_success();
    ASSERT_TRUE(res.ok());
    ASSERT_EQ(42, res.value());
}

TEST(result, Result_TakeError_fail)
{
    auto res = take_error_fail();
    ASSERT_TRUE(!res.ok());
    ASSERT_EQ("rough day again", res.error().message());
}

namespace
{

Result<double>
test_condition(const int x)
{
    TEST_CONDITION(x > 0);
    return 42;
}

Result<double>
test_condition_msg(const int x)
{
    TEST_CONDITION(x > 0) << "failed" << 42;
    return 42;
}

} // namespace

TEST(result, Result_TestCondition)
{
    ASSERT_TRUE(test_condition(1).ok());
    ASSERT_TRUE(test_condition(-1).bad());
}

TEST(result, Result_TestConditionMsg)
{
    ASSERT_TRUE(test_condition_msg(1).ok());
    ASSERT_TRUE(test_condition_msg(-1).bad());
}
