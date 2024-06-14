#include "traffic.h"
#include "io.h"
#include <cstdint>

int main()
{
	const uint64_t num_polygon_roadblocks = read<uint64_t>();
	const uint64_t num_chain_roadblocks   = read<uint64_t>();
	const uint64_t num_speed_zones        = read<uint64_t>();
	const uint64_t num_paths              = read<uint64_t>();

	if (num_paths == 0) return 0;

	TrafficZones traffic_zones = read_traffic_zones(
		num_polygon_roadblocks,
		num_chain_roadblocks,
		num_speed_zones
	);

	for (uint64_t i = 0; i < num_paths; ++i)
	{
		double og_speed    = read<uint16_t>();
		uint64_t num_nodes = read<uint64_t>();

		traffic_zones.for_each_affected_segment(og_speed, PathNodeReader(num_nodes), write_segment);
	}
}
