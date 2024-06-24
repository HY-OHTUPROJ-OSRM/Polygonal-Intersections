#include "interval_map.h"
#include <set>
#include <memory>
#include <memory_resource>
#include <functional>
#include <concepts>
#include <ranges>

template<class T>
static IntervalMap::Mapped value_before(T& start_points, typename T::const_iterator it)
{
	if (it == start_points.begin())
		return {};
	else
		return std::prev(it)->second;
}

void IntervalMap::insert(Rational start, Rational end, Mapped value)
{
	if (start >= end) return;

	auto overlap_begin = start_points.lower_bound(start);
	auto overlap_end   = start_points.upper_bound(end);

	const Mapped prev_value = value_before(start_points, overlap_begin);
	const Mapped next_value = value_before(start_points, overlap_end);

	start_points.erase(overlap_begin, overlap_end);

	if (value != prev_value)
		start_points.emplace(start, value);

	if (value != next_value)
		start_points.emplace(end, next_value);
}

void IntervalMap::insert_at_intersections(const LineSegment& ls, const Polygon& polygon, Mapped val)
{
	const std::size_t node_size = sizeof(std::pmr::set<Rational>::node_type);
	const std::size_t max_nodes = polygon.vertices.size() + 2;
	const std::size_t buffer_size = node_size * max_nodes;

	auto buffer = std::make_unique<std::byte[]>(buffer_size);

	std::pmr::monotonic_buffer_resource mbr(buffer.get(), buffer_size);
	std::pmr::polymorphic_allocator<Rational> allocator(&mbr);

	std::pmr::set<Rational> crossings(allocator);

	const Vector2 dir = ls.b - ls.a;

	auto visitor = [&]<class T>(T&& intersection, const LineSegment& edge)
	{
		if constexpr (std::same_as<T, Rational>)
		{
			if (intersection.is_zero()) return;

			if (Det(edge.a - ls.a, dir) < 0 || Det(edge.b - ls.a, dir) < 0)
			{
				// Add the intersection to the set, or remove it if it was already there
				const auto [it, inserted] = crossings.insert(intersection);

				if (!inserted) crossings.erase(it);
			}
		}
		else if constexpr (std::same_as<T, RationalInterval>)
			this->insert(intersection, val);
	};

	using namespace std::placeholders;

	for (LineSegment edge : polygon)
		std::visit(std::bind(visitor, _1, edge), find_intersection(ls, edge));

	if (polygon.limit_of_contains(ls.a, ls.b - ls.a))
		crossings.emplace(0);

	if (polygon.limit_of_contains(ls.b, ls.a - ls.b))
		crossings.emplace(1);

	for (auto it = crossings.begin();;)
	{
		if (it == crossings.end()) break;
		const Rational& start = *it++;

		if (it == crossings.end()) break;
		const Rational& end = *it++;

		if (start != end) this->insert(start, end, val);
	}
}
