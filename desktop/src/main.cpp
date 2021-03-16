#include <gcl.h>
#include <vca/result.h>
#include <iostream>

int main()
{
  auto t1 = gcl::task([]{ return 42; });
  auto t2 = gcl::task([]{ return 13.3; });
  auto t3 = gcl::tie(t1, t2).then([](auto t1, auto t2){ return *t1.get() + *t2.get(); });
  gcl::Async async{4};
  t3.schedule_all(async);
  t3.wait();
  std::cout << *t3.get() << std::endl; // 55.3
}
