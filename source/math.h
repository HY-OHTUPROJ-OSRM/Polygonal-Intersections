#pragma once
#include <cstdint>
#include <SFML/System/Vector2.hpp>

using Vector2 = sf::Vector2<int64_t>;

template<class T = int64_t>
inline sf::Vector2<T> ToVector2(auto... args)
{
	return {static_cast<T>(args)...};
}

template<class T = int64_t>
inline sf::Vector2<T> ToVector2(const auto& v)
{
	const auto [x, y] = v;

	return ToVector2<T>(x, y);
}

template<class T = int64_t>
inline auto Dot(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	return a.x * b.x + a.y * b.y;
}

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

	template<class T>
	constexpr explicit operator T() const
	{
		return static_cast<T>(nom) / den;
	}
};
