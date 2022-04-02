#include <array>
#include <iostream>

#include <docopt/docopt.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <spdlog/spdlog.h>

// This file will be generated automatically when you run the CMake
// configuration step. It creates a namespace called `Yes`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

int main(int argc, const char** argv)
{
    try {
        static constexpr auto USAGE{
            R"(Usage: intro [OPTIONS]

Options: -h --help    Show this screen.
         --version    Show version.
)"
        };

        std::map<std::string, docopt::value> args{ docopt::docopt(USAGE,
            { std::next(argv), std::next(argv, argc) },
            // show help if requested
            true,
            fmt::format("{} {}",
                Yes::cmake::project_name,
                // version string, acquired from config.hpp via CMake
                Yes::cmake::project_version)) };

    } catch (const std::exception& e) {
        fmt::print("Unhandled exception in main: {}", e.what());
    }
}
