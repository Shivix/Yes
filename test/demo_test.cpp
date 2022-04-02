#include <catch2/catch.hpp>

TEST_CASE("A demo test case")
{
    SECTION("Ensure 50 is not equal to 100")
    {
        REQUIRE(50 != 100);
        REQUIRE(20 != 100);
    }
}
