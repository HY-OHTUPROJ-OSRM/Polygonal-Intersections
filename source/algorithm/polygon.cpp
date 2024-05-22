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

static bool intersection_exists(const LineSegment& segment, const Polygon& polygon)
{
	for (LineSegment edge : polygon)
		if (find_intersection(edge, segment))
			return true;

	return false;
}

void PolygonalChain::for_each_intersecting_segment(const Polygon& polygon, void(&f)(const LineSegment& segment)) const
{
	if (vertices.empty()) return;

	bool inside = polygon.contains(vertices.front());

	for (LineSegment segment : *this)
	{
		if (intersection_exists(segment, polygon))
		{
			f(segment);

			inside = polygon.contains(segment.b);
		}
		else if (inside)
			f(segment);
	}
}
