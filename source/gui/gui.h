#pragma once
#include <SFML/Graphics.hpp>
#include "vector.h"
#include "polygon.h"

constexpr int screenWidth  = 1280;
constexpr int screenHeight = 720;
constexpr int screenMidX = screenWidth  / 2;
constexpr int screenMidY = screenHeight / 2;

inline const sf::Color markerColor = {156, 158, 232};

class DraggableButton : public sf::Drawable
{
	Vector2 pos;
	sf::Color color;
	Vector2 clickOffset = {};
	bool clicked = false;

public:
	DraggableButton(Vector2 pos, sf::Color color = markerColor):
		pos(pos), color(color)
	{}

	void Click(sf::Vector2i clickedPos);
	void Unclick() { clicked = false; }
	void UpdatePos(sf::Vector2i mousePos);

	const Vector2& GetPos() const { return pos; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

const sf::CircleShape& GetPointMarker(sf::Vector2f screenPos, sf::Color color = markerColor, bool filled = false);

const sf::CircleShape& GetPointMarker(auto pos, sf::Color color = markerColor, bool filled = false)
{
	return GetPointMarker(ToScreenCoordinates(pos), color, filled);
}

inline sf::Vector2f ToScreenCoordinates(Vector2 v)
{
	return {
		static_cast<float>(screenMidX + v.x),
		static_cast<float>(screenMidY - v.y)
	};
}

inline sf::Vector2f ToScreenCoordinates(Vector3 v)
{
	return {
		screenMidX + static_cast<float>(v.x) / v.z,
		screenMidY - static_cast<float>(v.y) / v.z
	};
}

inline Vector2 ToWorldCoordinates(const auto& v)
{
	return {
		static_cast<int64_t>(v.x - screenMidX),
		static_cast<int64_t>(screenMidY - v.y)
	};
}

inline void drawLabel(
	sf::RenderTarget& target,
	const char* string,
	const sf::Font& font,
	sf::Vector2f pos,
	sf::Vector2f offset,
	sf::Color color)
{
	sf::Text text(string, font);

	const auto endPos = pos + offset;

	text.setPosition(endPos.x + 5, endPos.y - 20);
	text.setFillColor(color);
	target.draw(text);

	const sf::Vertex indicator[] = {{pos, color}, {endPos, color}};

	target.draw(indicator, 2, sf::LineStrip);
}

template<class Base = PolygonalChain>
struct DrawablePolygonalChain : public Base, public sf::Drawable
{
	static constexpr bool circular = std::is_same_v<Base, Polygon>;

	using Base::points;
	sf::Color color;

	template<class... Args>
	DrawablePolygonalChain(sf::Color color, Args&&... args):
		Base(std::forward<Args>(args)...), color(color) {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		std::vector<sf::Vertex> verts;
		verts.reserve(points.size() + circular);

		for (auto& point : points)
			verts.emplace_back(ToScreenCoordinates(point), color);

		if (circular && verts.size() > 0)
			verts.push_back(verts.front());

		target.draw(verts.data(), verts.size(), sf::LineStrip, states);
	}
};

using DrawablePolygon = DrawablePolygonalChain<Polygon>;
