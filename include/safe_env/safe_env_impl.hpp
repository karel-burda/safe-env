#ifndef SAFE_ENV_SAFE_ENV_IMPL_HPP
#define SAFE_ENV_SAFE_ENV_IMPL_HPP

#include <cerrno>
#include <cstdlib>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

namespace burda::env
{
namespace detail
{
// due to performance reasons, we don't want to make this a static local variable
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline std::shared_mutex mtx;

template<typename F>
[[nodiscard]] std::string read(const std::string& name, const F getter)
{
    if (name.empty()) [[unlikely]]
    {
        throw std::invalid_argument{"Environment variable name is empty"};
    }

    std::shared_lock read_lock{mtx};

    if (char* result = getter(name.c_str())) [[unlikely]]
    {
        read_lock.unlock();

        return result;
    }

    return {};
}

template<typename F, typename... Args>
void write(const std::string& name, const F setter, Args&&... args)
{
    if (name.empty()) [[unlikely]]
    {
        throw std::invalid_argument{"Environment variable name is empty"};
    }

    std::unique_lock write_lock{mtx};

    if (const int ret = setter(name.c_str(), std::forward<Args>(args)...)) [[unlikely]]
    {
        // we can unlock, as errno is thread-safe
        write_lock.unlock();

        throw std::system_error{errno, std::generic_category(), "Failed to set environment variable " + name};
    }
}
} // namespace detail

// NOLINTNEXTLINE(misc-definitions-in-headers)
std::string getenv(const std::string& name)
{
    // NOLINTNEXTLINE(misc-include-cleaner); the "getenv" is in the <cstdlib>
    return detail::read(name, ::getenv);
}

// NOLINTNEXTLINE(misc-definitions-in-headers)
std::string secure_getenv(const std::string& name)
{
    // NOLINTNEXTLINE(misc-include-cleaner); the "secure_getenv" is in the <cstdlib>
    return detail::read(name, ::secure_getenv);
}

// NOLINTNEXTLINE(misc-definitions-in-headers)
void setenv(const std::string& name, const std::string& value, const bool overwrite)
{
    // NOLINTNEXTLINE(misc-include-cleaner); the "setenv" is in the <cstdlib>
    detail::write(name, ::setenv, value.c_str(), static_cast<int>(overwrite));
}

// NOLINTNEXTLINE(misc-definitions-in-headers)
void unsetenv(const std::string& name)
{
    // NOLINTNEXTLINE(misc-include-cleaner); the "unsetenv" is in the <cstdlib>
    detail::write(name, ::unsetenv);
}
} // namespace burda::env

#endif // SAFE_ENV_SAFE_ENV_IMPL_HPP
