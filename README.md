![Version](https://img.shields.io/badge/version-0.9.0-blue.svg)
![Language](https://img.shields.io/badge/C++20-blue.svg)
![Standard](https://img.shields.io/badge/POSIX-blue.svg)
![License](https://img.shields.io/badge/license-MIT_License-blue.svg?style=flat)
![CI](https://github.com/karel-burda/safe-env/actions/workflows/c-cpp.yml/badge.svg)

# Introduction
Simple C++ single-header **thread-safe** wrapper around `setenv`, `getenv`, `secure_getenv` and `unsetenv`.
`getenv` and `secure_getenv` functions return copy of `std::string`, so the return value might be safely manipulated with.

Wrapper throws C++ exceptions in case these OS function fails -- `std::system_error` or if called with wrong arguments (`std::invalid_argument`).

Aim of the wrapper is to make all calls **thread-safe**, which is implemented by an internal read-write lock.

Wrapper implements:
* `getenv`
* `secure_getenv`
* `setenv`
* `unsetenv`

Implemented and documented at the [safe_env.hpp](include/safe_env/safe_env.hpp).

# Example
```cpp
#include <safe_env/safe_env.hpp>

// all functions are safe to be called from different threads
burda::env::setenv("name", "value", true /* to overwrite any actual environment variable */);
burda::env::setenv("name", "value2", false /* will not overwrite */);
burda::env::getenv("name"); // returns std::string "value"
burda::env::unsetenv("name");
burda::env::getenv("name"); // returns an empty std::string
```

See also [main.cpp](main.cpp) for the test.
