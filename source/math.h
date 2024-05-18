#pragma once
#include <SFML/Graphics.hpp>

#include <cstdint>

constexpr unsigned screenWidth  = 1280;
constexpr unsigned screenHeight = 720;
constexpr unsigned screenMidX = screenWidth  / 2;
constexpr unsigned screenMidY = screenHeight / 2;

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
inline sf::Vector2f ToScreenCoordinates(const sf::Vector2<T>& v)
{
	return
	{
		static_cast<float>(screenMidX + v.x),
		static_cast<float>(screenMidY - v.y)
	};
}

template<class T = int64_t>
inline sf::Vector2<T> ToWorldCoordinates(const sf::Vector2<T>& v)
{
	return {v.x - screenMidX, screenMidY - v.y};
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

	constexpr explicit operator float() const
	{
		return static_cast<float>(nom) / den;
	}
};

/*
inline auto Lerp(const auto& a, const auto& b, Rational t)
{
	return t * (b - a) + a;
}
*/
