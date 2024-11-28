#ifndef BURDA_SAFE_ENV_HPP
#define BURDA_SAFE_ENV_HPP

#include <string>

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
} // namespace burda::env

#endif // BURDA_SAFE_ENV_HPP
