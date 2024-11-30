#include "safe_env/safe_env.hpp"

#undef NDEBUG

#include <cassert>
#include <chrono>
#include <future>
#include <thread>

// NOLINTNEXTLINE (google-build-using-namespace)
using namespace burda;

namespace
{
void test_single_threaded()
{
    assert(env::getenv("foo").empty());

    env::setenv("foo", "value", true);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value2", false);
    
    assert(env::getenv("foo") == "value");
    
    env::setenv("foo", "value2", true);

    assert(env::getenv("foo") == "value2");

    env::unsetenv("foo");

    assert(env::getenv("foo").empty());
}

void test_multi_threaded()
{
    const auto duration = std::chrono::seconds{30};

    auto setter = std::async(std::launch::async, [&]()
    {
        std::size_t iterations = 0;

        while(true)
        {
            env::setenv("key", "value", true);
            ++iterations;

            std::this_thread::yield();
        }

        return iterations;
    });

    std::this_thread::sleep_for(duration);

    assert(setter.get() > 0);
}
} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] const int argc, [[maybe_unused]] const char** argv)
{
    test_single_threaded();
    test_multi_threaded();
}
