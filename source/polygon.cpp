#include "polygonal_chain.h"

bool Polygon::contains(const Vector2& point) const
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
			continue;

		const Vector2 u = edge.b - edge.a;
		const Vector2 v = point - edge.a;

		const int64_t det = u.x * v.y - u.y * v.x;

		if (det > 0) continue; // if the edge is above the point
		if (det == 0) return true; // if the point is on the edge

		parity ^= 1; // otherwise the edge is below the point
	}

	return parity;
}
