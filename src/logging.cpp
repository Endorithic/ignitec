#include <fstream>

#include "logging.hpp"

Endo::Logger::Logger()
    : log_file(std::nullopt)
{ }

Endo::Logger::Logger(const std::string& path)
    : log_file(std::ofstream(path))
{ }
