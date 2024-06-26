#include "polygon.h"

bool Polygon::contains(Vector2 point) const
{
	int parity = 0;

	for (LineSegment edge : *this)
	{
		// If the edge is vertical
		if (edge.a.x == edge.b.x)
		{
			// If the point is on the vertical edge
			if (edge.a.x == point.x && (point.y - edge.a.y) * (point.y - edge.b.y) <= 0)
				return true;

			continue; // otherwise ignore
		}

		if (edge.a.x > edge.b.x)
			std::swap(edge.a, edge.b);

		// If no vertical line intersects both the point and
		// the edge, excluding the edge's rightmost point
		if (!(edge.a.x <= point.x && point.x < edge.b.x))
		{
			if (point == edge.b) return true;

			continue;
		}

		const int64_t det = Det(edge.b - edge.a, point - edge.a);

		if (det > 0) continue; // if the point is above the edge
		if (det == 0) return true; // if the point is on the edge

		parity ^= 1; // otherwise the edge is below the point
	}

	return parity;
}

bool Polygon::limit_of_contains(Vector2 point, Vector2 dir) const
{
	int parity = 0;

	for (LineSegment edge : *this)
	{
		// If the edge is parallel to dir
		if (Det(edge.b - edge.a, dir) == 0)
		{
			if (!edge.contains(point))
				continue;

			if (point == edge.a && Dot(dir, edge.a - edge.b) > 0)
				continue;

			if (point == edge.b && Dot(dir, edge.b - edge.a) > 0)
				continue;

			return true;
		}

		if (Det(edge.a - point, dir) >= 0 && Det(edge.b - point, dir) >= 0)
			continue;

		const auto edge_dir = edge.b - edge.a;

		const auto det = Det(edge_dir, dir);
		const auto offset = point - edge.a;

		const Rational t1 = {Det(offset,      dir), det};
		const Rational t2 = {Det(offset, edge_dir), det};

		if (!t1.is_negative() && !t1.is_above_1() && t2.is_positive())
			parity ^= 1;
	}

	return parity;
}

bool MultiPolygon::contains(Vector2 point) const
{
	for (auto& polygon : components)
		if (polygon.contains(point))
			return true;

	return false;
}
