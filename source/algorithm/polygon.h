#pragma once
#include "line_segment.h"
#include "vector.h"
#include "rational.h"
#include <concepts>
#include <vector>

struct BasePolygonalChain
{
	std::vector<Vector2_32> vertices;

	BasePolygonalChain() = default;
	BasePolygonalChain(std::initializer_list<Vector2_32> vertices) : vertices(vertices) {}

	template<class Range>
	BasePolygonalChain(Range&& range) : vertices(std::begin(range), std::end(range)) {}

	struct Iterator
	{
		decltype(vertices)::const_iterator start_point, end_point;

		LineSegment operator*() const { return {*start_point, *end_point}; }

		Iterator& operator++() & { return start_point = end_point++, *this; }

		bool operator==(const Iterator& other) const
		{
			return this->end_point == other.end_point;
		}
	};

	Iterator end() const & { return {vertices.end(), vertices.end()}; }
};

struct Polygon : BasePolygonalChain
{
	using BasePolygonalChain::BasePolygonalChain;

	Iterator begin() const &
	{
		if (vertices.begin() == vertices.end())
			return {vertices.end(), vertices.end()};
		else
			return {std::prev(vertices.end()), vertices.begin()};
	}

	bool contains(Vector2 point) const;

	// Returns true iff every neighborhood of the point contains
	// a point inside the polygon in the given direction
	bool limit_of_contains(Vector2 point, Vector2 dir) const;
};

struct PolygonalChain : BasePolygonalChain
{
	using BasePolygonalChain::BasePolygonalChain;

	Iterator begin() const &
	{
		if (vertices.begin() == vertices.end())
			return {vertices.end(), vertices.end()};
		else
			return {vertices.begin(), std::next(vertices.begin())};
	}
};

template<std::derived_from<BasePolygonalChain> Component>
struct MultiShape
{
	std::vector<Component> components;

	MultiShape() = default;
	MultiShape(std::initializer_list<Component> components) : components(components) {}

	bool edge_intersects(const LineSegment& segment) const
	{
		for (auto& c : components)
			for (LineSegment edge : c)
				if (find_intersection(edge, segment).index())
					return true;

		return false;
	}
};

struct MultiPolygon : MultiShape<Polygon>
{
	using MultiShape::MultiShape;

	bool contains(Vector2 point) const;
};

using MultiPolygonalChain = MultiShape<PolygonalChain>;
