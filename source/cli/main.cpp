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

void read_vertices(BasePolygonalChain& poly_chain)
{
	const uint64_t num_vertices = read<uint64_t>();

	for (uint64_t i = 0; i < num_vertices; ++i)
	{
		const auto x = read<int64_t>();
		const auto y = read<int64_t>();

		poly_chain.vertices.emplace_back(x, y);
	}
}

int main()
{
	const uint64_t num_polygons   = read<uint64_t>();
	const uint64_t num_polychains = read<uint64_t>();

	std::vector<Polygon>        polygons;
	std::vector<PolygonalChain> polychains;

	polygons.reserve(num_polygons);
	polychains.reserve(num_polychains);

	for (uint64_t i = 0; i < num_polygons; ++i)
		read_vertices(polygons.emplace_back());

	for (uint64_t i = 0; i < num_polychains; ++i)
		read_vertices(polychains.emplace_back());

	for (const auto& polygon : polygons)
	{
		for (const auto& polychain : polychains)
		{
			if (auto intersection = polychain.find_first_intersection(polygon))
				std::cout << "An intersection exists\n";
		}
	}
}
