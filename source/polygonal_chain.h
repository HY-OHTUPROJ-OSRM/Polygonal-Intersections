#pragma once
#include "line_segment.h"
#include <vector>

using PointContainer = std::vector<Vector2>;

struct PolygonalIterator
{
	PointContainer::const_iterator start_point, end_point;
	sf::Color color;

	LineSegment operator*() const
	{
		return {*start_point, *end_point, color};
	}

	PolygonalIterator& operator++() & { return start_point = end_point++, *this; }

	constexpr bool operator==(const PolygonalIterator& other) const
	{
		return this->end_point == other.end_point;
	}

	static PolygonalIterator circular_begin(const std::vector<Vector2>& points, sf::Color color)
	{
		if (points.begin() == points.end())
			return {points.end(), points.end(), color};
		else
			return {std::prev(points.end()), points.begin(), color};
	}

	static PolygonalIterator linear_begin(const std::vector<Vector2>& points, sf::Color color)
	{
		if (points.begin() == points.end())
			return {points.end(), points.end(), color};
		else
			return {points.begin(), std::next(points.begin()), color};
	}
};

template<decltype(PolygonalIterator::linear_begin) begin_func>
struct BasePolygonalChain : public sf::Drawable
{
	PointContainer points;
	sf::Color color;

	template<class It>
	BasePolygonalChain(It begin, It end, sf::Color color):
		points(begin, end), color(color) {}

	template<class Range>
	BasePolygonalChain(Range&& range, sf::Color color):
		points(std::begin(range), std::end(range)), color(color) {}

	constexpr PolygonalIterator begin() const & { return begin_func(points, color); }
	constexpr PolygonalIterator end()   const & { return {points.end(), points.end(), color}; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (LineSegment segment : *this)
		{
			segment.color = color;
			target.draw(segment);
		}

		(void)states;
	}
};

static_assert(sizeof(sf::Color) == 4);

using Polygon        = BasePolygonalChain<PolygonalIterator::circular_begin>;
using PolygonalChain = BasePolygonalChain<PolygonalIterator::linear_begin>;
