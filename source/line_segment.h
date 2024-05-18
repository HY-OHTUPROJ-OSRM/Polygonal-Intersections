#pragma once
#include "math.h"
#include <optional>

struct LineSegment : public sf::Drawable
{
	Vector2 a, b;
	sf::Color color;

	LineSegment(const Vector2& a, const Vector2& b, sf::Color color):
		a(a), b(b), color(color)
	{}

	sf::Vector2<Rational> eval(Rational t) const
	{
		const Vector2 diff = b - a;

		return {
			Rational{t.den * a.x + t.nom * diff.x, t.den},
			Rational{t.den * a.y + t.nom * diff.y, t.den}
		};
	}

	// Checks if the segment contains the orthogonal projection of the given point
	bool contains_projection(const Vector2& point) const
	{
		if (a == b) return point == a;

		const Vector2 diff = b - a;

		return Dot(point - a, diff) >= 0 && Dot(point - b, diff) <= 0;
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		const auto verts = std::array
		{
			sf::Vertex{ToScreenCoordinates(a), color},
			sf::Vertex{ToScreenCoordinates(b), color}
		};
		
		target.draw(verts.data(), verts.size(), sf::Lines, states);

		(void)states;
	}
};

std::optional<sf::Vector2<Rational>> find_intersection(const LineSegment& ls1, const LineSegment& ls2);
