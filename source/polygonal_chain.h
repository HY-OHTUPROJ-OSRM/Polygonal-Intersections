#pragma once
#include "line_segment.h"
#include "math.h"
#include <vector>

struct BasePolygonalChain
{
	std::vector<Vector2> points;

	template<class It>
	BasePolygonalChain(It begin, It end) : points(begin, end) {}

	template<class Range>
	BasePolygonalChain(Range&& range) : points(std::begin(range), std::end(range)) {}

	struct Iterator
	{
		decltype(points)::const_iterator start_point, end_point;

		LineSegment operator*() const { return {*start_point, *end_point}; }

		Iterator& operator++() & { return start_point = end_point++, *this; }

		bool operator==(const Iterator& other) const
		{
			return this->end_point == other.end_point;
		}
	};

	Iterator end() const & { return {points.end(), points.end()}; }
};

struct Polygon : public BasePolygonalChain
{
	Iterator begin() const &
	{
		if (points.begin() == points.end())
			return {points.end(), points.end()};
		else
			return {std::prev(points.end()), points.begin()};
	}

	bool contains(const Vector2& point) const;
};

struct PolygonalChain : public BasePolygonalChain
{
	Iterator begin() const &
	{
		if (points.begin() == points.end())
			return {points.end(), points.end()};
		else
			return {points.begin(), std::next(points.begin())};
	}

	std::optional<sf::Vector2<Rational>> find_first_intersection(const Polygon& polygon) const;
};
