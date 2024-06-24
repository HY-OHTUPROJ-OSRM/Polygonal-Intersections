#pragma once
#include "traffic.h"
#include <iostream>
#include <optional>
#include <cstdint>

template<class T>
T read()
{
	T res;
	std::cin.read(reinterpret_cast<char*>(&res), sizeof(res));
	return res;
}

void write(auto val)
{
	std::cout.write(reinterpret_cast<char*>(&val), sizeof(val));
}

struct PathNodeReader
{
	uint64_t num_nodes;

	std::optional<PathNode> operator()();
};

uint64_t read_vertices(BasePolygonalChain& base_poly_chain);

void write_segment(const PathNode& start, const PathNode& end, double new_speed);

TrafficZones read_traffic_zones(
	uint64_t num_polygon_roadblocks,
	uint64_t num_chain_roadblocks,
	uint64_t num_speed_zones);
