#include "safe_env/safe_env.hpp"

#include <cerrno>
#include <cstdlib>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace
{
std::shared_mutex mtx;

template<typename F>
std::string burda::env::detail::read(const std::string& name, const F getter)
{
    if (name.empty()) [[unlikely]]
    {
        throw std::invalid_argument{"Environment variable name is empty"};
    }

    std::shared_lock read_lock{mtx};

    if (char* result = getter(name.c_str()))
    {
        read_lock.unlock();

        return result;
    }

    return {};
} // namespace anonymous

template<typename F, typename... Args>
void burda::env::detail::write(const std::string& name, const F setter, Args&&... args)
{
    if (name.empty()) [[unlikely]]
    {
        throw std::invalid_argument{"Environment variable name is empty"};
    }

    std::unique_lock write_lock{mtx};

    if (const int ret = setter(std::forward<Args>(args)...)) [[unlikely]]
    {
        // we can unlock, as errno is thread-safe
        write_lock.unlock();

        throw std::system_error{errno, std::generic_category(), "Failed to set environment variable " + name};
    }
}
} // namespace anonymous

namespace burda::env
{
std::string burda::env::getenv(const std::string& name)
{
    return burda::env::detail::read(name, ::getenv);
}

std::string burda::env::secure_getenv(const std::string& name)
{
    return burda::env::detail::read(name, ::secure_getenv);
}

void burda::env::setenv(const std::string& name, const std::string& value, bool overwrite)
{
    burda::env::detail::write(name, ::setenv, value, static_cast<int>(overwrite));
}

void burda::env::unsetenv(const std::string& name)
{
   burda::env::detail::write(name, ::unsetenv);
}
} // namespace burda::env
