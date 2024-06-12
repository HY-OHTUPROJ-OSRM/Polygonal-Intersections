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

	constexpr bool operator>(auto other) const
	{
		return other < *this;
	}

	constexpr bool operator<=(auto other) const
	{
		return !(other < *this);
	}

	constexpr bool operator>=(auto other) const
	{
		return !(*this < other);
	}

	constexpr bool is_positive() const
	{
		return (nom > 0 && den > 0) || (nom < 0 && den < 0);
	}

	constexpr bool is_negative() const
	{
		return (nom < 0 && den > 0) || (nom > 0 && den < 0);
	}

	constexpr bool is_zero() const
	{
		return nom == 0;
	}

	constexpr bool is_above_1() const
	{
		if (den > 0)
			return nom > den;
		else
			return nom < den;
	}

	template<class T>
	constexpr explicit operator T() const
	{
		return static_cast<T>(nom) / den;
	}
};

struct RationalInterval
{
	Rational start, end;

	constexpr RationalInterval(int64_t start_nom, int64_t end_nom, int64_t den):
		start(start_nom, den), end(end_nom, den) {}

	constexpr RationalInterval(Rational start, Rational end):
		start(start), end(end) {}
};
