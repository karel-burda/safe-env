#include "safe_env/safe_env.hpp"

#include <caasert>

using namespace burda;

namespace
{
int test()
{
    env::setenv("foo", "value", 1);
    assert(env::get_env("foo"), "value");
}
} // namespace anonymous

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char** argv)
{
  return test();
}
