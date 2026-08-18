#include "init.hpp"
#include "function_map.hpp"
#include "logging.hpp"

/// Temporary function signature, will eventually have an argument parser instead of std::string
using FunctionMap = Endo::FunctionMap<int, Endo::Logger&, const std::optional<std::string>&>;

int main(int argc, char** argv) {
    // We don't need the program invocation line, so we just ignore it.
    // Argc is almost always at least 1, but it is technically allowed to be 0,
    // so we guard against that.
    if (argc > 0) {
        --argc; ++argv;
    }

    Endo::Logger logger{};

    if (argc < 1) {
        logger.error("Please provide a subcommand.");
        return 1;
    }

    FunctionMap function_map = FunctionMap::create();
    function_map.insert("init", Endo::init);

    const std::string subcommand = argv[0];
    const std::optional<std::string> argument = argc >= 2 ? std::optional<std::string>(argv[1]) : std::nullopt;

    const auto ret = function_map.execute(subcommand, logger, argument);
    if (!ret) {
        /// In the future, this will handle the actual ExecuteError.
        /// For now, we just print an error
        logger.error("Subcommand `{}` not recognized.", subcommand);
        return 3;
    }

    return *ret;
}
