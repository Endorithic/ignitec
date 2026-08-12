#include "get_config.hpp"

constexpr std::string_view AUTHOR_NAME = "Endorithic"; // Windows convention is `Appdata\Roaming\AUTHOR_NAME\PROJECT_NAME`
constexpr std::string_view PROJECT_NAME = "ignitec";

#ifdef __linux__

#include <cstdlib>

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

#endif // __linux__

#ifdef _WIN32

#include <windows.h>
#include <ShlObj.h>
#include <combaseapi.h>

std::optional<std::filesystem::path> Endo::get_config() {
    PWSTR path = nullptr;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
    if (SUCCEEDED(hr)) {
        std::filesystem::path final_path{ path };
        CoTaskMemFree(path);
        return final_path / AUTHOR_NAME / PROJECT_NAME;
    } else {
        return std::nullopt;
    }
}

#endif // _WIN32
