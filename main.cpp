#include "safe_env/safe_env.hpp"

#include <cassert>

using namespace burda;

namespace
{
void test_single_threaded()
{
    env::setenv("foo", "value", 1);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value", 0);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value2", 1);

    assert(env::getenv("foo") == "value");
}

void test_multi_threaded()
{
}
} // namespace anonymous

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char** argv)
{
    test_single_threaded();
    test_multi_threaded();
}
