#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include "internal/generator.h"

namespace grafology {
    /** @brief a grafology exception
     * @details This exception allows the definition of errors specific to this library.
     */
    class error : public std::runtime_error {
       public:
        // inherit the constructors
        using std::runtime_error::runtime_error;
        // allow to use std::format directly in the constructor
        template <class... Args>
        error(std::format_string<Args...> fmt, Args&&... args)
            : std::runtime_error(std::format(fmt, std::forward<Args>(args)...)) {}
    };

}  // namespace grafology