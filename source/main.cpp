#include "ui.h"
#include "math.h"
#include "polygonal_chain.h"

#include <array>
#include <iostream>
#include <ranges>
#include <SFML/Graphics.hpp>

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

	while (window.isOpen()) try
	{
		window.clear(bgColor);
		const Vector2 mousePos = ToVector2(sf::Mouse::getPosition(window));
		
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
					button.Click(ToVector2(event.mouseButton.x, event.mouseButton.y));
				break;
			case sf::Event::MouseButtonReleased:
				for (auto& button : buttons)
					button.Unclick();
				break;
			}
		}

		const PolygonalChain polygonalChain {
			buttons |
			std::views::take(5) |
			std::views::transform(&DraggableButton::GetPos),
			{250, 0, 30}
		};

		const Polygon polygon {
			buttons |
			std::views::drop(5) |
			std::views::transform(&DraggableButton::GetPos),
			{0, 255, 30}
		};

		window.draw(polygon);
		window.draw(polygonalChain);

		for (LineSegment ls1 : polygonalChain)
			for (LineSegment ls2 : polygon)
				if (auto intersection = find_intersection(ls1, ls2))
					window.draw(GetPointMarker(*intersection, sf::Color::Yellow));

		for (auto& button : buttons)
		{
			button.UpdatePos(mousePos);
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
