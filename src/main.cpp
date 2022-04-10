#include <docopt/docopt.h>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <spdlog/spdlog.h>

#include <random>

#include "instruments.hpp"

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
        static constexpr int node_size{ 7 };

        canvas.DrawPointCircle(center, center, outer_size - 1);
        canvas.DrawPointCircle(center, center, outer_size);
        canvas.DrawPointCircle(center, center, outer_size + 1);

        canvas.DrawPointCircle(center, center, middle_size - 1);
        canvas.DrawPointCircle(center, center, middle_size);
        canvas.DrawPointCircle(center, center, middle_size + 1);

        canvas.DrawPointCircle(center, center, inner_size - 1);
        canvas.DrawPointCircle(center, center, inner_size);
        canvas.DrawPointCircle(center, center, inner_size + 1);

        using std::numbers::pi;

        // node positions
        const auto outer_x{ static_cast<int>(center + outer_size * std::cos(pi + offsets[0])) };
        const auto outer_y{ static_cast<int>(center + outer_size * std::sin(pi + offsets[0])) };
        canvas.DrawPointCircleFilled(outer_x, outer_y, node_size, ftxui::Color::Blue);

        const auto middle_x{ static_cast<int>(center + middle_size * std::cos(pi + offsets[1])) };
        const auto middle_y{ static_cast<int>(center + middle_size * std::sin(pi + offsets[1])) };
        canvas.DrawPointCircleFilled(middle_x, middle_y, node_size, ftxui::Color::Purple);

        const auto inner_x{ static_cast<int>(center + inner_size * std::cos(pi + offsets[2])) };
        const auto inner_y{ static_cast<int>(center + inner_size * std::sin(pi + offsets[2])) };
        canvas.DrawPointCircleFilled(inner_x, inner_y, node_size, ftxui::Color::Red);

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

        std::array<double, 3> ring_offsets{ 0, 0, 0 };
        std::size_t current_ring{ 0 };
        auto screen{ ftxui::ScreenInteractive::TerminalOutput() };

        // randomise the correct pin position
        std::random_device random_device;
        std::mt19937 random_engine(random_device());
        std::uniform_int_distribution<int> endpoint_rng(1, 6);// NOLINT
        std::array<int, 3> endpoints{
            endpoint_rng(random_engine), endpoint_rng(random_engine), endpoint_rng(random_engine)
        };

        const auto cylinder{ ftxui::Renderer(make_lock_cylinder(ring_offsets)) };
        const auto graph{ ftxui::Renderer(make_plot_graph(current_ring, ring_offsets, endpoints)) };
        const auto text_box{ ftxui::Renderer(make_info_box(current_ring)) };

        // std::chrono::system_clock::time_point start_time{};
        auto start_time{ std::chrono::high_resolution_clock::now() };
        const auto game_logic{ ftxui::Renderer([&] {
            if (current_ring >= ring_offsets.size()) {
                screen.ExitLoopClosure()();
                if (check_victory(ring_offsets, endpoints)) {
                    const auto duration{ std::chrono::high_resolution_clock::now() - start_time };
                    const auto score{ std::to_string(
                        calculate_score(ring_offsets, endpoints, duration)) };
                    return ftxui::hbox(
                        ftxui::text("YOU WIN, SCORE WAS: " + score + "\n (the lower the better)")
                        | ftxui::borderDouble);
                }
                return ftxui::hbox(ftxui::text("SORRY, YOU FAILED. GAME OVER")) | ftxui::borderDouble;
            }
            return ftxui::hbox(cylinder->Render(),
                ftxui::vbox(graph->Render() | ftxui::borderDouble,
                    text_box->Render() | ftxui::borderDouble));
        }) };

        const auto event_handler{ ftxui::CatchEvent(
            game_logic, [&current_ring, &ring_offsets](const ftxui::Event& event) {
                using ftxui::Event;
                static constexpr auto delta{ 0.03 };
                if (event == Event::Character(' ')) { ++current_ring; }
                if (event == Event::Character('a') || event == Event::Character('h')) {
                    ring_offsets.at(current_ring) -= delta;
                }
                if (event == Event::Character('d') || event == Event::Character('l')) {
                    ring_offsets.at(current_ring) += delta;
                }
                return false;
            }) };
        start_time = std::chrono::high_resolution_clock::now();
        screen.Loop(event_handler);

        return 0;

    } catch (const std::exception& e) {
        fmt::print("Unhandled exception in main: {}", e.what());
    }
}
