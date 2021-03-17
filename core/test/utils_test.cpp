#include <gtest/gtest.h>

#include <vca/utils.h>

TEST(utils, ignore_op)
{
    vca::ignore_op();
    vca::ignore_op(1.3, 42);
}

namespace
{

struct Base
{
    virtual ~Base() = default;
};

struct Derived : Base
{
};

} // namespace

TEST(utils, downcast_with_nullptr)
{
    Base* x = nullptr;
    ASSERT_EQ(nullptr, vca::downcast<Derived>(x));
}

TEST(utils, downcast_happy_path)
{
    std::unique_ptr<Base> x = std::make_unique<Derived>();
    ASSERT_EQ(x.get(), vca::downcast<Derived>(x.get()));
}
