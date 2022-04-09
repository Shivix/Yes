#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <array>
#include <chrono>
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

[[nodiscard]] constexpr long calculate_score(const std::array<double, 3>& offsets,
    const std::array<int, 3>& endpoints,
    std::chrono::system_clock::duration duration)
{
    long score{};
    const auto max_pin_accuracy{ 2000.0 };
    for (std::size_t i{ 0 }; i < offsets.size(); ++i) {
        score += static_cast<long>(
            max_pin_accuracy
            - (check_pin_position(offsets.at(i), endpoints.at(i)) * 100.0));// NOLINT
    }
    score *= duration.count();
    return score;
}

#endif// GAME_LOGIC_HPP
