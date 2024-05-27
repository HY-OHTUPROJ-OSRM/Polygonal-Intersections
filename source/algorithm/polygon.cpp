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

		const Vector2 u = edge.b - edge.a;
		const Vector2 v = point - edge.a;

		const int64_t det = u.x * v.y - u.y * v.x;

		if (det > 0) continue; // if the edge is above the point
		if (det == 0) return true; // if the point is on the edge

		parity ^= 1; // otherwise the edge is below the point
	}

	return parity;
}

std::optional<Vector3> PolygonalChain::find_first_intersection(const Polygon& polygon) const
{
	if (!vertices.empty() && polygon.contains(vertices.front()))
		return {{vertices.front().x, vertices.front().y, 1}};

	for (LineSegment ls1 : *this)
	{
		std::optional<Rational> first_intersection;

		for (LineSegment ls2 : polygon)
		{
			const auto intersection = find_intersection(ls1, ls2);

			if (intersection && (!first_intersection || *intersection < *first_intersection))
				first_intersection = intersection;
		}

		if (first_intersection)
			return {ls1.eval(*first_intersection)};
	}

	return std::nullopt;
}

bool MultiPolygon::contains(Vector2 point) const
{
	for (auto& polygon : polygons)
		if (polygon.contains(point))
			return true;

	return false;
}

bool MultiPolygon::edge_intersects(const LineSegment& segment) const
{
	for (auto& polygon : polygons)
		for (LineSegment edge : polygon)
			if (find_intersection(edge, segment))
				return true;

	return false;
}
