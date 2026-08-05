#ifndef ENDO_FUNCTION_MAP_HPP
#define ENDO_FUNCTION_MAP_HPP

#include <type_traits>
#include <unordered_map>
#include <string>
#include <expected>

template <typename Ty>
concept NotVoid = !std::is_void_v<Ty>;

namespace Endo {

enum class ExecuteError {
    /// Indicates that a function was not found in the map.
    FunctionNotFound = 0,
};

/// HashMap based function lookup and executor.
/// NOTE: Template type `Ret` cannot be `void`
template <NotVoid Ret, typename... Args>
class FunctionMap {
    using FunctionPointer = Ret(*)(Args...);

private:
    /// Maps function names as `std::string` to function pointers
    std::unordered_map<std::string, FunctionPointer> functions;

    /// Default constructor; should not be used by the user.
    FunctionMap()
        : functions()
    { }

public:
    /// Creates a new empty function map.
    /// Note: Only one function map can be created per template instantiation. This is enforced
    /// using a singleton pattern.
    static FunctionMap& create() {
        static FunctionMap map;
        return map;
    }

    /// Executes the function. Returns an expected type containing a possible error
    /// describing how the function execution failed.
    template <typename... Callargs>
    [[nodiscard]] std::expected<Ret, ExecuteError> execute(const std::string& name, Callargs&&... args) {
        auto it = this->functions.find(name);
        if (it != this->functions.end()) {
            return it->second(std::forward<Callargs>(args)...);
        }

        return std::unexpected(ExecuteError::FunctionNotFound);
    }

    /// Inserts a function into the function map.
    void insert(const std::string& name, FunctionPointer pointer) {
        if (!this->functions.contains(name)) {
            this->functions[name] = pointer;
        }
    }
};

} // namespace Endo

#endif // ENDO_FUNCTION_MAP_HPP
