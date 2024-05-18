#pragma once
#include "line_segment.h"
#include "math.h"
#include <vector>

template<bool circular = false>
struct PolygonalChain
{
	std::vector<Vector2> points;

	template<class It>
	constexpr PolygonalChain(It begin, It end):
		points(begin, end) {}

	template<class Range>
	constexpr PolygonalChain(Range&& range):
		points(std::begin(range), std::end(range)) {}

	struct Iterator
	{
		decltype(points)::const_iterator start_point, end_point;

		LineSegment operator*() const { return {*start_point, *end_point}; }

		constexpr Iterator& operator++() & { return start_point = end_point++, *this; }

		constexpr bool operator==(const Iterator& other) const
		{
			return this->end_point == other.end_point;
		}
	};

	constexpr Iterator begin() const &
	{
		if (points.begin() == points.end())
			return {points.end(), points.end()};

		if constexpr (circular)
			return {std::prev(points.end()), points.begin()};
		else
			return {points.begin(), std::next(points.begin())};
	}

	constexpr Iterator end() const & { return {points.end(), points.end()}; }
};

using Polygon = PolygonalChain<true>;
