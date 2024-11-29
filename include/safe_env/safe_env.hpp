#ifndef BURDA_SAFE_ENV_HPP
#define BURDA_SAFE_ENV_HPP

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
std::string getenv(const std::string& name);

std::string secure_getenv(const std::string& name);

/// @brief Searches for a given key, aimed to return associated value with it.
/// @param key key to be searched for
/// @return pair, where first denotes whether element was found, second given value
/// @throws 
/// @details Deliberately not throwing an exception, and returning pair instead,
///          as this generates much shorter assembly on clang and msvc
void setenv(const std::string& name, const std::string& value, bool overwrite);

void unsetenv(const std::string& name);

namespace detail
{
inline std::shared_mutex mtx;

template<typename F>
std::string read(const std::string& name, const F getter);

template<typename F, typename... Args>
void write(const std::string& name, const F setter, Args&&... args);
} // namespace burda::env::detail
} // namespace burda::env

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
    burda::env::detail::write(name, ::setenv, value.c_str(), static_cast<int>(overwrite));
}

void burda::env::unsetenv(const std::string& name)
{
   burda::env::detail::write(name, ::unsetenv);
}

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
}

template<typename F, typename... Args>
void burda::env::detail::write(const std::string& name, const F setter, Args&&... args)
{
    if (name.empty()) [[unlikely]]
    {
        throw std::invalid_argument{"Environment variable name is empty"};
    }

    std::unique_lock write_lock{mtx};

    if (const int ret = setter(name, std::forward<Args>(args)...)) [[unlikely]]
    {
        // we can unlock, as errno is thread-safe
        write_lock.unlock();

        throw std::system_error{errno, std::generic_category(), "Failed to set environment variable " + name};
    }
}

#endif // BURDA_SAFE_ENV_HPP
