#include "gui.h"

#include <array>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <windows.h>

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
	GetWindowsDirectory(windows_dir, MAX_PATH);

	sf::Font main_font;
	main_font.loadFromFile(std::string(windows_dir) + std::string("\\Fonts\\seguisb.ttf"));

	auto buttons = std::to_array<DraggableButton>
	({
		// Polygonal chain
		Vector2 {-201,  -310},
		Vector2 {-154,  -204},
		Vector2 {-109,   -96},
		Vector2 { -53,     3},
		Vector2 {   5,   109},
		
		// Polygon
		Vector2 { 200,    0},
		Vector2 { 190,  150},
		Vector2 {-100,  200},
		Vector2 {-150,   50},
		Vector2 {   0, -200},
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

		DrawablePolygonalChain polygonalChain {
			{250, 0, 30},
			buttons |
			std::views::take(5) |
			std::views::transform(&DraggableButton::GetPos),
		};

		DrawablePolygon polygon {
			{0, 255, 30},
			buttons |
			std::views::drop(5) |
			std::views::transform(&DraggableButton::GetPos),
		};

		Vector2 mousePos = ToWorldCoordinates(mouseScreenPos);

		if (snappingEnabled)
		{
			const int64_t mask = ~0x1fll;
			auto snap = [](auto& p) { p.x &= mask; p.y &= mask; };
			std::ranges::for_each(polygon.points, snap);
			std::ranges::for_each(polygonalChain.points, snap);
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
		window.draw(polygonalChain);

		if (!polygonalChain.points.empty())
		{
			drawLabel(
				window,
				"Start",
				main_font,
				ToScreenCoordinates(polygonalChain.points.front()),
				sf::Vector2f{30, 30},
				markerColor
			);

			drawLabel(
				window,
				"End",
				main_font,
				ToScreenCoordinates(polygonalChain.points.back()),
				sf::Vector2f{30, 30},
				markerColor
			);
		}

		for (LineSegment ls1 : polygonalChain)
			for (LineSegment ls2 : polygon)
				if (auto intersection = find_intersection(ls1, ls2))
					window.draw(GetPointMarker(ls1.eval(*intersection), sf::Color::Yellow));

		const char* status = "No intersections";

		if (auto first_intersection = polygonalChain.find_first_intersection(polygon))
		{
			drawLabel(
				window,
				"First intersection",
				main_font,
				ToScreenCoordinates(*first_intersection),
				sf::Vector2f{30, -30},
				sf::Color::Yellow
			);

			status = "At least one intersection exists";
		}

		sf::Text statusText(status, main_font);
		statusText.setPosition(30, 30);
		window.draw(statusText);

		for (auto& button : buttons)
		{
			button.UpdatePos(mouseScreenPos);
			window.draw(button);
		}

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
