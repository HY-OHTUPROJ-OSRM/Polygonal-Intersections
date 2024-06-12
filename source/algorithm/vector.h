#pragma once
#include <cstdint>

template<class T>
struct Vector2_Impl
{
	T x, y;

	constexpr Vector2_Impl() = default;

	constexpr Vector2_Impl(T x, T y) : x(x), y(y) {}

	template<class U>
	constexpr Vector2_Impl(Vector2_Impl<U> v) : x(v.x), y(v.y) {}

	constexpr bool operator==(const Vector2_Impl&) const = default;
};

using Vector2    = Vector2_Impl<int64_t>;
using Vector2_32 = Vector2_Impl<int32_t>;

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

constexpr int64_t Det(Vector2 a, Vector2 b)
{
	return a.x * b.y - a.y * b.x;
}
