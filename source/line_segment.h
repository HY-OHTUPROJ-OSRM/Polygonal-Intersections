#pragma once
#include "math.h"
#include <optional>

struct LineSegment
{
	Vector2 a, b;

	constexpr LineSegment(const Vector2& a, const Vector2& b) : a(a), b(b) {}

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
};

std::optional<Rational> find_intersection(const LineSegment& ls1, const LineSegment& ls2);
