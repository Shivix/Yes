#define CATCH_CONFIG_MAIN// This tells the catch header to generate a main

#include <catch2/catch.hpp>

TEST_CASE("Example test case")
{
    SECTION("Ensure 0 is not equal to 1") { REQUIRE(0 != 1); }
}
