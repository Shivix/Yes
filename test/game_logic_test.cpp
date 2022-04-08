#include <catch2/catch.hpp>

#include "game_logic.hpp"

TEST_CASE("game logic test case")
{
    SECTION("check victory condition")
    {
        using std::numbers::pi;
        const std::array<double, 3> test_offsets{ pi, pi, pi };
        const std::array<int, 3> test_endpoints{ 0, 0, 0 };
        REQUIRE(check_victory(test_offsets, test_endpoints));
    }
}
