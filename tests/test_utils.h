#pragma once
#include "rational.h"
#include "vector.h"
#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const Rational& rational)
{
	return os << rational.nom << '/' << rational.den;
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	return os << '{' << v.x << ", " << v.y << '}';
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	return os << '{' << v.x << ", " << v.y << ", " << v.z << '}';
}
