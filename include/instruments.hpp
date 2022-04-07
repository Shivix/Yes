#ifndef INSTRUMENTS_HPP
#define INSTRUMENTS_HPP

#include <ftxui/component/component.hpp>

#include <array>
#include <cmath>

[[nodiscard]] constexpr auto make_info_box(const std::size_t& ring)
{
    return [&] {
        static constexpr int canvas_size{ 160 };
        auto canvas{ ftxui::Canvas(canvas_size, canvas_size / 2) };

        if (ring == 0) {
            canvas.DrawText(0, 0, "Here is text for ring 1");
        } else if (ring == 1) {
            canvas.DrawText(0, 0, "Here is text for ring 2");
        } else {
            canvas.DrawText(0, 0, "Here is text for the final ring");
        }

        return ftxui::canvas(std::move(canvas));
    };
}

[[nodiscard]] constexpr auto make_plot_graph(const std::size_t& ring,
    std::array<double, 3>& offsets)
{
    return [&] {
        static constexpr int canvas_size{ 160 };
        auto canvas{ ftxui::Canvas(canvas_size, canvas_size / 2) };

        std::vector<int> graph_points{};
        static constexpr int max_plots{ 160 };
        graph_points.reserve(max_plots);

        for (std::size_t i{ 0 }; i < max_plots; ++i) {
            const auto point{ static_cast<double>(i) - offsets.at(ring) * 40 };
            static constexpr auto zero_point{ 50.0 };
            static constexpr auto spike_height{ 20.0 };
            static constexpr auto frequency{ 0.14 };
            static constexpr auto floor_thing{ 10.0 };
            static constexpr auto spikiness{ 0.42 };
            graph_points.emplace_back(static_cast<int>(
                zero_point + (offsets.at(ring) * spike_height) * std::cos(point * frequency)
                + floor_thing * std::sin(point * spikiness)));
        }

        for (std::size_t i{ 1 }; i < max_plots - 1; ++i) {
            const int line_start{ graph_points[i] };
            const int line_end{ graph_points[i + 1] };
            canvas.DrawPointLine(
                static_cast<int>(i), line_start, static_cast<int>(i + 1), line_end);
        }

        return ftxui::canvas(std::move(canvas));
    };
}

#endif
