#pragma once
#include "vector.h"
#include "rational.h"
#include <optional>

struct LineSegment
{
	Vector2 a, b;

	constexpr LineSegment(Vector2 a, Vector2 b) : a(a), b(b) {}

	// Returns the result of linear interpolation in homogeneous coordinates
	constexpr Vector3 eval(Rational t) const
	{
		const Vector2 diff = b - a;

		return {
			t.den * a.x + t.nom * diff.x,
			t.den * a.y + t.nom * diff.y,
			t.den
		};
	}

	// Checks if the segment contains the orthogonal projection of the given point
	constexpr bool contains_projection(Vector2 point) const
	{
		if (a == b) return point == a;

		const Vector2 diff = b - a;

		return Dot(point - a, diff) >= 0 && Dot(point - b, diff) <= 0;
	}
};

std::optional<Rational> find_intersection(const LineSegment& ls1, const LineSegment& ls2);
