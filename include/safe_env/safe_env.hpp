#ifndef SAFE_ENV_SAFE_ENV_HPP
#define SAFE_ENV_SAFE_ENV_HPP

#include "safe_env/safe_env_impl.hpp"

#include <string>

/// @brief Free functions that wrap work with environment variables in a thread-safe manner.
namespace burda::env
{
/// @brief Reads environment variable using "getenv" in a thread-safe manner.
/// @param name name of environment variable
/// @return Environment variable value or an empty string
/// @throws std::invalid_argument if name is empty, std::system_error
inline [[nodiscard]] std::string getenv(const std::string& name);

/// @brief Reads environment variable using "secure_getenv" in a thread-safe manner.
/// @param name name of environment variable
/// @return Environment variable value or an empty string
/// @see burda::env::getenv
inline [[nodiscard]] std::string secure_getenv(const std::string& name);

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
} // namespace burda::env

#endif // SAFE_ENV_SAFE_ENV_HPP
