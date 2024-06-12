#pragma once
#include "rational.h"
#include "line_segment.h"
#include "polygon.h"
#include <map>

struct IntervalMap
{
	using Mapped = uint64_t;

	struct Value : RationalInterval
	{
		Mapped mapped;

		constexpr Value(Rational start, Rational end, Mapped mapped):
			RationalInterval(start, end), mapped(mapped) {}
	};

	std::map<Rational, Mapped> start_points;

	void insert(Rational start, Rational end, Mapped val);

	void insert(const RationalInterval& interval, Mapped val)
	{
		insert(interval.start, interval.end, val);
	}

	void insert_at_intersections(const LineSegment& ls, const Polygon& polygon, Mapped val);

	struct Iterator
	{
		std::map<Rational, Mapped>::const_iterator it;

		Value operator*() const
		{
			const auto prev = std::prev(it);

			return {prev->first, it->first, prev->second};
		}

		Iterator& operator++() { ++it; return *this; }

		bool operator==(const Iterator&) const = default;
	};

	Iterator begin() const
	{
		auto val = start_points.begin();

		if (val == start_points.end())
			return {val};
		else
			return {std::next(val)};
	}

	Iterator end() const { return {start_points.end()}; }
};
