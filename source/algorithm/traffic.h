#pragma once
#include "polygon.h"
#include <cstdint>
#include <vector>

struct SpeedZone : Polygon
{
	enum Type : uint8_t { offset, factor, cap, constant } type;

	double speed_value;

	SpeedZone(Type type, double speed_value):
		type(type), speed_value(speed_value) {}

	double transform_speed(double speed) const;
};

struct PathNode
{
	Vector2_32 pos;
	uint64_t id;
};

struct TrafficZones
{
	MultiPolygon roadblock_polygons;
	MultiPolygonalChain roadblock_polychains;
	std::vector<SpeedZone> speed_zones;

	void for_each_affected_segment(double og_speed, auto&& read_node, auto&& write_segment)
	{
		auto first_node = read_node();
		if (!first_node) return;

		PathNode start = *first_node;
		bool in_roadblock = roadblock_polygons.contains(start.pos);

		while (true)
		{
			auto next_node = read_node();
			if (!next_node) break;

			const PathNode& end = *next_node;
			LineSegment segment = {start.pos, end.pos};
			double new_speed = og_speed;

			if (roadblock_polygons.edge_intersects(segment))
			{
				new_speed = 0;

				in_roadblock = roadblock_polygons.contains(end.pos);
			}
			else if (in_roadblock || roadblock_polychains.edge_intersects(segment))
				new_speed = 0;
			else
				new_speed = segment.new_average_speed(og_speed, speed_zones);

			if (new_speed != og_speed)
				write_segment(start, end, new_speed);

			start = end;
		}
	}
};
