#pragma once
#include "math.h"
#include <cmath>
#include <optional>
#include <cassert>

struct LineSegment : public sf::Drawable
{
	Vector2 a, b;
	sf::Color color;

	LineSegment(const Vector2& a, const Vector2& b, sf::Color color):
		a(a), b(b), color(color)
	{}

	sf::Vector2<Rational> eval(Rational t) const
	{
		Vector2 diff = b - a;

		return {
			Rational{t.den * a.x + t.nom * diff.x, t.den},
			Rational{t.den * a.y + t.nom * diff.y, t.den}
		};
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

std::optional<sf::Vector2<Rational>> find_intersection(const LineSegment& ls1, const LineSegment& ls2)
{
	const auto dir1 = ls1.b - ls1.a;
	const auto dir2 = ls2.b - ls2.a;

	const auto det = dir1.x * dir2.y - dir2.x * dir1.y;

	if (det == 0)
	{
		// TODO: Check if lines are collinear and overlap?

		return std::nullopt;
	}

	const auto offset = ls2.a - ls1.a;

	const Rational t1 = {dir2.y * offset.x - dir2.x * offset.y, det};
	const Rational t2 = {dir1.y * offset.x - dir1.x * offset.y, det};

	if (t1.nom * det < 0 ||
		t2.nom * det < 0 ||
		t1.nom * det > det * det ||
		t2.nom * det > det * det
	)
		return std::nullopt;

	assert(ls1.eval(t1) == ls2.eval(t2));

	return ls1.eval(t1);
}
