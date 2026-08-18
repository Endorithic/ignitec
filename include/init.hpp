#ifndef ENDO_INIT_HPP
#define ENDO_INIT_HPP

#include <optional>

#include "logging.hpp"

namespace Endo {

/// Creates a project directory in the CWD and initializes the project structure.
int init(Logger& logger, const std::optional<std::string>& project_name);

}

#endif
