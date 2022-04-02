#include <docopt/docopt.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <spdlog/spdlog.h>

#include <array>
#include <cmath>
#include <numbers>

[[nodiscard]] constexpr auto make_lock_cylinder(const std::array<double, 3>& offsets)
{
    return [&] {
        static constexpr int center{ 80 };
        static constexpr int canvas_size{ 160 };
        auto canvas{ ftxui::Canvas(canvas_size, canvas_size) };

        // rings
        static constexpr int outer_size{ 70 };
        static constexpr int middle_size{ 45 };
        static constexpr int inner_size{ 20 };
        static constexpr int pin_size{ 7 };
        canvas.DrawPointCircle(center, center, outer_size);
        canvas.DrawPointCircle(center, center, middle_size);
        canvas.DrawPointCircle(center, center, inner_size);

        using std::numbers::pi;
        static constexpr double PI_2{ pi / 2 };
        static constexpr double TAU_3{ 2 * pi / 3 };

        for (auto i{ 1 }; i <= 4; ++i) {
            const auto pin_x{ static_cast<int>(
                center + outer_size * std::cos(PI_2 * (i + offsets[0]))) };
            const auto pin_y{ static_cast<int>(
                center + outer_size * std::sin(PI_2 * (i + offsets[0]))) };
            canvas.DrawPointCircleFilled(pin_x, pin_y, pin_size, ftxui::Color::Blue);
        }
        for (auto i{ 1 }; i <= 3; ++i) {
            const auto pin_x{ static_cast<int>(
                center + middle_size * std::cos(TAU_3 * (i + offsets[1]))) };
            const auto pin_y{ static_cast<int>(
                center + middle_size * std::sin(TAU_3 * (i + offsets[1]))) };
            canvas.DrawPointCircleFilled(pin_x, pin_y, pin_size, ftxui::Color::Purple);
        }
        for (auto i{ 1 }; i <= 2; ++i) {
            const auto pin_x{ static_cast<int>(
                center + inner_size * std::cos(pi * (i + offsets[2]))) };
            const auto pin_y{ static_cast<int>(
                center + inner_size * std::sin(pi * (i + offsets[2]))) };
            canvas.DrawPointCircleFilled(pin_x, pin_y, pin_size, ftxui::Color::Red);
        }
        return ftxui::canvas(std::move(canvas));
    };
}

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

        std::array<double, 3> ring_offsets{ 0.0, std::numeric_limits<double>::max_digits10, 0.0 };
        std::size_t current_ring{ 0 };
        auto screen{ ftxui::ScreenInteractive::TerminalOutput() };

        const auto cylinder{ ftxui::Renderer(make_lock_cylinder(ring_offsets)) };

        const auto game_logic{ ftxui::Renderer([&] {
            if (current_ring >= ring_offsets.size()) {
                screen.ExitLoopClosure()();
                return ftxui::hbox(ftxui::text("GAME OVER")) | ftxui::border;
            }
            return cylinder->Render();
        }) };

        const auto event_handler{ ftxui::CatchEvent(
            game_logic, [&current_ring, &ring_offsets](const ftxui::Event& event) {
                using ftxui::Event;
                static constexpr auto delta{ 0.02 };
                if (event == Event::Character(' ')) { ++current_ring; }
                if (event == Event::Character('a')) { ring_offsets.at(current_ring) -= delta; }
                if (event == Event::Character('d')) { ring_offsets.at(current_ring) += delta; }
                return false;
            }) };

        screen.Loop(event_handler);

        return 0;
    } catch (const std::exception& e) {
        fmt::print("Unhandled exception in main: {}", e.what());
    }
}
