#include "safe_env/safe_env.hpp"

#undef NDEBUG

#include <cassert>

namespace
{
void test_single_threaded()
{
    // NOLINTNEXTLINE
    using namespace burda;

    env::setenv("foo", "value", true);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value", false);
    
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
