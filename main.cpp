#include "safe_env/safe_env.hpp"

#undef NDEBUG

#include <cassert>

// NOLINTNEXTLINE (google-build-using-namespace)
using namespace burda;

namespace
{
void test_single_threaded()
{
    assert(env::getenv("foo") == "");

    env::setenv("foo", "value", true);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value2", false);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value2", true);

    assert(env::getenv("foo") == "value");
}

void test_multi_threaded()
{
}
} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] const int argc, [[maybe_unused]] const char** argv)
{
    test_single_threaded();
    test_multi_threaded();
}
