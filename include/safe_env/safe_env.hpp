#ifndef SAFE_ENV_SAFE_ENV_HPP
#define SAFE_ENV_SAFE_ENV_HPP

#include <cerrno>
#include <cstdlib>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

/// @brief Free functions that wrap work with environment variables in a thread-safe manner.
namespace burda::env
{
/// @brief Reads environment variable using "getenv" in a thread-safe manner.
/// @param name name of environment variable
/// @return Environment variable value or an empty string
/// @throws std::invalid_argument if name is empty, std::system_error
inline std::string getenv(const std::string& name);

/// @brief Reads environment variable using "secure_getenv" in a thread-safe manner.
/// @param name name of environment variable
/// @return Environment variable value or an empty string
/// @see burda::env::getenv
inline std::string secure_getenv(const std::string& name);

/// @brief Sets environment variable using "setenv" in a thread-safe manner.
/// @param name name of environment variable
/// @param value desired value
/// @param overwrite do we want to overwrite an existing value?
/// @throws std::invalid_argument if name is empty, std::system_error
inline void setenv(const std::string& name, const std::string& value, bool overwrite);

/// @brief Unsets environment variable using "unsetenv" in a thread-safe manner.
/// @param name name of environment variable to be removed/unset
/// @throws std::invalid_argument if name is empty, std::system_error
inline void unsetenv(const std::string& name);

/// @brief Sets environment variables using "putenv" in a thread-safe manner.
/// @param key_value_pairs features string in form of "env1=value1,env2=value2"
/// @throws std::invalid_argument if name is empty, std::system_error
inline void putenv(const std::string& key_value_pairs);

namespace detail
{
// due to performance reasons, we don't want to make this a static local variable
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
inline std::shared_mutex mtx;

template<typename F>
std::string read(const std::string& name, F getter);

template<typename F, typename... Args>
void write(const std::string& name, F setter, Args&&... args);
} // namespace detail
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

void burda::env::putenv(const std::string& key_value_pairs)
{
    burda::env::detail::write(name, ::putenv, key_value_pairs.c_str());
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

    if (const int ret = setter(name.c_str(), std::forward<Args>(args)...)) [[unlikely]]
    {
        // we can unlock, as errno is thread-safe
        write_lock.unlock();

        throw std::system_error{errno, std::generic_category(), "Failed to set environment variable " + name};
    }
}

#endif // SAFE_ENV_SAFE_ENV_HPP
