#ifndef CG_ERROR_HPP_
#define CG_ERROR_HPP_

#include <concepts>
#include <string>
#include <variant>

template <typename T>
concept Error = requires(const T error) {
    { error.stringify() } -> std::same_as<std::string>;
};

template <Error... Errors>
[[nodiscard]] inline std::string stringify_error_variant(
    const std::variant<Errors...>& error_variant
) noexcept
{
    return std::visit(
        [](Error auto const& err) { return err.stringify(); },
        error_variant
    );
}

#endif // !CG_ERROR_HPP_
