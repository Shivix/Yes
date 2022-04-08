#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <array>
#include <cmath>
#include <numbers>

// NOLINTNEXTLINE WHAT DO ABOUT IT?
[[nodiscard]] inline double check_pin_position(double offset, double endpoint)
{
    // NOLINTNEXTLINE my understanding of math sucks, it really is a magic number to me sorry
    return 10 + 10 * std::cos(std::numbers::pi + endpoint + offset);
}

[[nodiscard]] constexpr auto check_victory(const std::array<double, 3>& offsets,
    const std::array<int, 3>& endpoints)
{
    const auto victory_margin{ 18.0 };
    for (std::size_t i{ 0 }; i < offsets.size(); ++i) {
        if (check_pin_position(offsets.at(i), endpoints.at(i)) < victory_margin) { return false; }
    }
    return true;
}

#endif// GAME_LOGIC_HPP
