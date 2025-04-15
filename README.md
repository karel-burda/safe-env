![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)
![Language](https://img.shields.io/badge/C++20-blue.svg)
![Standard](https://img.shields.io/badge/POSIX-blue.svg)
![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)
![CI](https://github.com/karel-burda/safe-env/actions/workflows/c-cpp.yml/badge.svg)

# Introduction
Simple C++ header-only **thread-safe** wrapper around `setenv`, `getenv`, `secure_getenv` and `unsetenv`.
`getenv` and `secure_getenv` functions return copy of `std::string`, so the return value might be safely manipulated with.

Wrapper throws C++ exceptions: `std::system_error` if OS specific functions failed and `std::invalid_argument` if called with wrong arguments (empty variable name).

Aim of the wrapper is to make all calls **thread-safe**, which is implemented by an internal read-write lock.

Wrapper implements:
* `getenv`
* `secure_getenv`
* `setenv`
* `unsetenv`

Documented and implemented at the [safe_env.hpp](include/safe_env/safe_env.hpp), [safe_env_impl.hpp](include/safe_env/safe_env)impl.hpp) resp..

# Example
```cpp
#include <safe_env/safe_env.hpp>

// all functions are safe to be called from different threads
burda::env::setenv("env_name", "env_value", true /* overwrite any actual environment variable */);
burda::env::setenv("env_name", "env_value2", false /* will not overwrite */);
std::string env_value = burda::env::getenv("env_name"); // contains "env_value"
burda::env::unsetenv("env_name");
env_value = burda::env::getenv("env_name"); // contains an empty std::string
```

See also [main.cpp](main.cpp) for the test example.
