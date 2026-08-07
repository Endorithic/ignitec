#include <cstdlib>

#include "get_config.hpp"

constexpr std::string PROJECT_NAME = "ignitec";

#ifdef __linux__

std::optional<std::filesystem::path> Endo::get_config() {
    const char* xdg_config = std::getenv("XDG_CONFIG_HOME");
    if (xdg_config != nullptr && *xdg_config != '\0') {
        return std::filesystem::path(xdg_config) / PROJECT_NAME;
    }

    const char* home = std::getenv("HOME");
    if (home != nullptr && *home != '\0') {
        return std::filesystem::path(home) / ".config" / PROJECT_NAME;
    }

    return std::nullopt;
}

#endif
