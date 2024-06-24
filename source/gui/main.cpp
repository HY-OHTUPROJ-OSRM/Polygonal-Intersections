#include "gui.h"

#include <array>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "polygon.h"
#include "interval_map.h"

namespace Windows {
#include <windows.h>
}

using std::cout;

sf::RenderWindow* windowPtr;

int main()
{
	sf::err().rdbuf(nullptr); // disable sfml errors

	sf::RenderWindow window =
	{
		sf::VideoMode {screenWidth, screenHeight},
		"Polygonal Intersections",
		sf::Style::Titlebar | sf::Style::Close
	};

	windowPtr = &window;
	const sf::Color bgColor = {30, 32, 44};

	window.clear(bgColor);
	window.display();
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);

	char windows_dir[MAX_PATH];
	Windows::GetWindowsDirectory(windows_dir, MAX_PATH);

	sf::Font main_font;
	main_font.loadFromFile(std::string(windows_dir) + std::string("\\Fonts\\seguisb.ttf"));

	auto buttons = std::to_array<DraggableButton>
	({
		// Polygonal chain
		Vector2 {-201,  -310},
		Vector2 {-154,  -204},
		Vector2 {-109,   -96},
		Vector2 { 153,   -70},
		Vector2 {   5,   109},
		Vector2 { 256,    40},
		Vector2 { 260,   160},
		Vector2 { 185,   237},
		Vector2 { 243,   328},
		Vector2 { 358,   302},

		// Polygon
		Vector2 { 200,  200},
		Vector2 { 190,  150},
		Vector2 {-100,  200},
		Vector2 {-150,   50},
		Vector2 {   0, -200},
		Vector2 { 300, -200},
		Vector2 { 400,    0},
		Vector2 { 300,  150},
		Vector2 { 200,  300},
		Vector2 {  50,  250}
	});

	bool snappingEnabled = false;
	while (window.isOpen()) try
	{
		window.clear(bgColor);
		const auto mouseScreenPos = sf::Mouse::getPosition(window);
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				for (auto& button : buttons)
					button.Click({event.mouseButton.x, event.mouseButton.y});
				break;
			case sf::Event::MouseButtonReleased:
				for (auto& button : buttons)
					button.Unclick();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space)
					snappingEnabled ^= true;
				break;
			}
		}

		DrawablePolygonalChain polychain {
			{250, 0, 30},
			buttons |
			std::views::take(10) |
			std::views::transform(&DraggableButton::GetPos),
		};

		DrawablePolygon polygon {
			{0, 255, 30},
			buttons |
			std::views::drop(10) |
			std::views::transform(&DraggableButton::GetPos),
		};

		Vector2 mousePos = ToWorldCoordinates(mouseScreenPos);

		if (snappingEnabled)
		{
			const int64_t mask = ~0x1fll;
			auto snap = [](auto& p) { p.x &= mask; p.y &= mask; };
			std::ranges::for_each(polygon.vertices, snap);
			std::ranges::for_each(polychain.vertices, snap);
			snap(mousePos);
		}

		const bool inside = polygon.contains(mousePos);

		drawLabel(
			window,
			inside ? "Inside" : "Outside",
			main_font,
			ToScreenCoordinates(mousePos),
			sf::Vector2f{30, -30},
			inside ? sf::Color{0, 255, 30} : markerColor
		);

		window.draw(polygon);
		window.draw(polychain);

		if (!polychain.vertices.empty())
		{
			drawLabel(
				window,
				"Start",
				main_font,
				ToScreenCoordinates(polychain.vertices.front()),
				sf::Vector2f{30, 30},
				markerColor
			);

			drawLabel(
				window,
				"End",
				main_font,
				ToScreenCoordinates(polychain.vertices.back()),
				sf::Vector2f{30, 30},
				markerColor
			);
		}

		for (LineSegment ls1 : polychain)
		{
			for (LineSegment ls2 : polygon)
			{
				std::visit([&]<class T>(T&& x)
				{
					if constexpr (std::same_as<T, Rational>)
						window.draw(GetPointMarker(ls1.eval(x), sf::Color::Yellow));

				}, find_intersection(ls1, ls2));
			}
		}

		for (auto& button : buttons)
		{
			button.UpdatePos(mouseScreenPos);
			window.draw(button);
		}

		for (LineSegment segment : polychain)
		{
			IntervalMap intervals;
			intervals.insert_at_intersections(segment, polygon, 1);

			for (const auto& part : intervals)
			{
				if (part.mapped == 0) continue;

				sf::Vertex arr[]
				{
					{ToScreenCoordinates(segment.eval(part.start)), sf::Color::Blue},
					{ToScreenCoordinates(segment.eval(part.end)),   sf::Color::Blue}
				};

				window.draw(arr, 2, sf::LineStrip);
			}
		}

		/*
		polychain.for_each_intersecting_segment(MultiPolygon{{polygon}}, [&](std::size_t segment_id)
		{
			const sf::Vertex line[] = {
				{ToScreenCoordinates(polychain.vertices[segment_id]),     sf::Color::White},
				{ToScreenCoordinates(polychain.vertices[segment_id + 1]), sf::Color::White}
			};

			window.draw(line, 2, sf::LineStrip);
		});
		*/

		window.display();
	}
	catch (std::exception& e)
	{
		cout << "Exception:\n" << e.what();
		cout << "\nPress any key to continue\n";

		while (window.isOpen())
		{
			sf::Event event;
			window.waitEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			else if (event.type == sf::Event::KeyPressed)
				break;
		}
	}
}
