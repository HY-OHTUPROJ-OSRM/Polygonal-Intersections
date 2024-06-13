#pragma once
#include "vector.h"
#include "rational.h"
#include <cmath>
#include <vector>
#include <variant>

struct SpeedZone;

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

	constexpr bool contains(Vector2 point) const
	{
		return contains_projection(point) && Det(point - a, b - a) == 0;
	}

	double length() const { return std::hypot(b.x - a.x, b.y - a.y); }

	double new_average_speed(double original_speed, const std::vector<SpeedZone>& speed_zones) const;
};

// Finds the intersection in terms of parameters to ls1
std::variant<std::monostate, Rational, RationalInterval>
find_intersection(const LineSegment& ls1, const LineSegment& ls2);
