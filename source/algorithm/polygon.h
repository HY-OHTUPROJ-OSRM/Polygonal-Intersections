#pragma once
#include "line_segment.h"
#include "vector.h"
#include "rational.h"
#include <utility>
#include <vector>

struct BasePolygonalChain
{
	std::vector<Vector2> vertices;

	BasePolygonalChain() = default;

	template<class It>
	BasePolygonalChain(It begin, It end) : vertices(begin, end) {}

	template<class Range>
	BasePolygonalChain(Range&& range) : vertices(std::begin(range), std::end(range)) {}

	BasePolygonalChain(std::vector<Vector2>&& vertices) : vertices(std::move(vertices)) {}

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

struct Polygon : public BasePolygonalChain
{
	Iterator begin() const &
	{
		if (vertices.begin() == vertices.end())
			return {vertices.end(), vertices.end()};
		else
			return {std::prev(vertices.end()), vertices.begin()};
	}

	bool contains(Vector2 point) const;
};

struct MultiPolygon
{
	std::vector<Polygon> polygons;

	bool contains(Vector2 point) const
	{
		for (auto& polygon : polygons)
			if (polygon.contains(point))
				return true;

		return false;
	}
};

bool intersection_exists(const LineSegment& segment, const MultiPolygon& multipolygon);

struct PolygonalChain : public BasePolygonalChain
{
	Iterator begin() const &
	{
		if (vertices.begin() == vertices.end())
			return {vertices.end(), vertices.end()};
		else
			return {vertices.begin(), std::next(vertices.begin())};
	}

	std::optional<Vector3> find_first_intersection(const Polygon& polygon) const;
	
	void for_each_intersecting_segment(const MultiPolygon& multipolygon, auto&& f) const
	{
		if (vertices.empty()) return;

		bool inside = multipolygon.contains(vertices.front());

		for (std::size_t segment_id = 0; segment_id + 1 < vertices.size(); ++segment_id)
		{
			const auto& a = vertices[segment_id];
			const auto& b = vertices[segment_id + 1];

			if (intersection_exists(LineSegment{a, b}, multipolygon))
			{
				f(segment_id);

				inside = multipolygon.contains(b);
			}
			else if (inside)
				f(segment_id);
		}
	}
};
