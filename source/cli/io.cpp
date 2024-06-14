#include "io.h"

std::optional<PathNode> PathNodeReader::operator()()
{
	if (num_nodes == 0)
		return std::nullopt;

	--num_nodes;

	const auto id = read<uint64_t>();
	const auto x  = read<int32_t>();
	const auto y  = read<int32_t>();

	return PathNode {{x, y}, id};
}

uint64_t read_vertices(BasePolygonalChain& base_poly_chain)
{
	const uint64_t num_vertices = read<uint64_t>();

	for (uint64_t i = 0; i < num_vertices; ++i)
	{
		const auto x = read<int32_t>();
		const auto y = read<int32_t>();

		base_poly_chain.vertices.emplace_back(x, y);
	}

	return num_vertices;
}

void write_segment(const PathNode& start, const PathNode& end, double new_speed)
{
	write<uint64_t>(start.id);
	write<uint64_t>(end.id);
	write<double>(new_speed);
}

TrafficZones read_traffic_zones(
	uint64_t num_polygon_roadblocks,
	uint64_t num_chain_roadblocks,
	uint64_t num_speed_zones)
{
	TrafficZones traffic_zones;

	auto& polygon_roadblocks = traffic_zones.roadblock_polygons.components;
	auto& chain_roadblocks = traffic_zones.roadblock_polychains.components;
	auto& speed_zones = traffic_zones.speed_zones;

	polygon_roadblocks.reserve(num_polygon_roadblocks);
	chain_roadblocks.reserve(num_chain_roadblocks);
	speed_zones.reserve(num_speed_zones);

	for (uint64_t i = 0; i < num_polygon_roadblocks; ++i)
		read_vertices(polygon_roadblocks.emplace_back());

	for (uint64_t i = 0; i < num_chain_roadblocks; ++i)
		read_vertices(chain_roadblocks.emplace_back());

	for (uint64_t i = 0; i < num_speed_zones; ++i)
	{
		SpeedZone::Type type = read<SpeedZone::Type>();
		double speed_value = read<double>();

		read_vertices(speed_zones.emplace_back(type, speed_value));
	}

	return traffic_zones;
}
