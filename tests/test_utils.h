#pragma once
#include "rational.h"
#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const Rational& rational)
{
	return os << rational.nom << '/' << rational.den;
}
