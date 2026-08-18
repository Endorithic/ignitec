#include <filesystem>

#include "init.hpp"
#include "logging.hpp"

namespace fs = std::filesystem;

int Endo::init(Logger& logger, const std::optional<std::string>& project_name) {
    if (!project_name) {
        logger.error("Please provide a project name.");
        return 1;
    }

    const fs::path cwd = fs::current_path();

    fs::path project_directory = cwd / *project_name;
    if (fs::exists(project_directory)) {
        logger.error("Directory `{}` already exists.", *project_name);
        return 1;
    }

    logger.info("Creating directory `{}`", *project_name);
    bool did_create_project = fs::create_directory(project_directory);
    if (!did_create_project) {
        logger.error("Failed to create project directory.");
        return 1;
    }

    fs::path src_dir = project_directory / "src";
    fs::path include_dir = project_directory / "include";

    logger.info("Creating project directories");
    bool did_create_src = fs::create_directory(src_dir);
    bool did_create_include = fs::create_directory(include_dir);
    bool did_create_proj_include = fs::create_directory(include_dir / *project_name);
    if (!did_create_src || !did_create_include || !did_create_proj_include) {
        logger.error("Failed to create source directories.");
        return 1;
    }

    logger.info("Creating `src/main.cpp`");
    std::ofstream main_cpp(src_dir / "main.cpp");
    if (!main_cpp.is_open()) {
        logger.error("Failed to open `main.cpp`");
        return 1;
    }

    return 0;
}
