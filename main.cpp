#include "safe_env/safe_env.hpp"

#undef NDEBUG

#include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <thread>

// NOLINTNEXTLINE (google-build-using-namespace)
using namespace burda;

namespace
{
void test_exceptions()
{
    try
    {
        env::getenv("");
    }
    catch (const std::invalid_argument& error)
    {
        assert(error.what() == "test");
    }
    catch (...)
    {
        // we expect std::invalid_argument
        assert(false);
    }
}

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
    std::atomic_bool run { true };

    const auto runner = [&](const auto function)
    {
        std::size_t iterations = 0;

        while(run)
        {
            function();
            ++iterations;

            std::this_thread::yield();
        }

        return iterations;
    };

    auto setter = std::async(std::launch::async, [&](){ return runner([&]()
    {
        env::setenv("key", "value", true);
    });});

    auto reader = std::async(std::launch::async, [&](){ return runner([&]()
    {
        const std::string value = env::getenv("key");

        assert(value.empty() || value == "value");
    });});

    auto deleter = std::async(std::launch::async, [&](){ return runner([&]()
    {
        env::unsetenv("key");
    });});

    std::this_thread::sleep_for(duration);

    run = false;

    assert(setter.get() > 0);
    assert(reader.get() > 0);
    assert(deleter.get() > 0);
}
} // namespace

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] const int argc, [[maybe_unused]] const char** argv)
{
    test_exceptions();
    test_single_threaded();
    test_multi_threaded();
}
