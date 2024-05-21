#pragma once
#include <cstdint>

struct Rational
{
	int64_t nom, den = 1;

	constexpr Rational operator+(Rational other) const
	{
		return {this->nom * other.den + this->den * other.nom, this->den * other.den};
	}

	constexpr Rational operator-(Rational other) const
	{
		return {this->nom * other.den - this->den * other.nom, this->den * other.den};
	}

	constexpr Rational operator*(Rational other) const
	{
		return {this->nom * other.nom, this->den * other.den};
	}

	constexpr Rational operator/(Rational other) const
	{
		return {this->nom * other.den, this->den * other.nom};
	}

	constexpr bool operator==(Rational other) const
	{
		return this->nom * other.den == this->den * other.nom;
	}

	constexpr bool operator<(Rational other) const
	{
		if (this->den * other.den > 0)
			return this->nom * other.den < this->den * other.nom;
		else
			return this->nom * other.den > this->den * other.nom;
	}

	constexpr bool operator>(Rational other) const
	{
		return other < *this;
	}

	constexpr bool operator<=(Rational other) const
	{
		return !(other < *this);
	}

	constexpr bool operator>=(Rational other) const
	{
		return !(*this < other);
	}

	template<class T>
	constexpr explicit operator T() const
	{
		return static_cast<T>(nom) / den;
	}
};
