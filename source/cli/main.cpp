#include <iostream>
#include <vector>
#include "polygon.h"
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

uint64_t read_vertices(BasePolygonalChain& poly_chain)
{
	const uint64_t num_vertices = read<uint64_t>();

	for (uint64_t i = 0; i < num_vertices; ++i)
	{
		const auto x = read<int64_t>();
		const auto y = read<int64_t>();

		poly_chain.vertices.emplace_back(x, y);
	}

	return num_vertices;
}

struct NodeAwarePolygonalChain : public PolygonalChain
{
	std::vector<uint64_t> node_ids;

	void read_node_ids(uint64_t n)
	{
		for (uint64_t i = 0; i < n; ++i)
			node_ids.push_back(read<uint64_t>());
	}
};

int main()
{
	const uint64_t num_polygons   = read<uint64_t>();
	const uint64_t num_polychains = read<uint64_t>();

	MultiPolygon multipolygon;
	std::vector<NodeAwarePolygonalChain> polychains;

	multipolygon.polygons.reserve(num_polygons);
	polychains.reserve(num_polychains);

	for (uint64_t i = 0; i < num_polygons; ++i)
		read_vertices(multipolygon.polygons.emplace_back());

	for (uint64_t i = 0; i < num_polychains; ++i)
	{
		auto& polychain = polychains.emplace_back();

		const auto num_vertices = read_vertices(polychain);
		polychain.read_node_ids(num_vertices);
	}

	for (const auto& polychain : polychains)
	{
		polychain.for_each_intersecting_segment(multipolygon, [&](std::size_t segment_id)
		{
			write(polychain.node_ids[segment_id]);
			write(polychain.node_ids[segment_id + 1]);
		});
	}
}
