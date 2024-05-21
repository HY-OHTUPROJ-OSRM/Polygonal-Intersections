#pragma once
#include <cstdint>

struct Vector2
{
	int64_t x, y;

	constexpr bool operator==(const Vector2&) const = default;
};

struct Vector3
{
	int64_t x, y, z;

	constexpr bool operator==(const Vector3&) const = default;
};

constexpr Vector2 operator+(Vector2 u, Vector2 v)
{
	return {u.x + v.x, u.y + v.y};
}

constexpr Vector2 operator-(Vector2 u, Vector2 v)
{
	return {u.x - v.x, u.y - v.y};
}

constexpr Vector3 operator+(Vector3 u, Vector3 v)
{
	return {u.x + v.x, u.y + v.y, u.z + v.z};
}

constexpr Vector3 operator-(Vector3 u, Vector3 v)
{
	return {u.x - v.x, u.y - v.y, u.z - v.z};
}

constexpr int64_t Dot(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}
