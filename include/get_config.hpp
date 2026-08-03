#ifndef ENDO_GET_CONFIG_HPP
#define ENDO_GET_CONFIG_HPP

#include <filesystem>
#include <optional>

namespace Endo {

/// Returns the platform dependent project configuration directory.
/// On Linux, this is usually `~/.config/ignitec`.
/// If the directory cannot be located, the function returns `std::nullopt`.
[[nodiscard]] std::optional<std::filesystem::path> get_config();

}

#endif
