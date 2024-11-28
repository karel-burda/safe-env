#include "safe_env/safe_env.hpp"

using namespace burda;

int test()
{
    env::set("foo", "value", 1);
}

int main([[maybe_unsed]] const int argc, [[maybe_unused]] const char** argv)
{
  return test();
}
