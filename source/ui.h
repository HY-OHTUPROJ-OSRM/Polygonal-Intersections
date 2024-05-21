#pragma once
#include <SFML/Graphics.hpp>
#include "math.h"
#include "polygonal_chain.h"

constexpr unsigned screenWidth  = 1280;
constexpr unsigned screenHeight = 720;
constexpr unsigned screenMidX = screenWidth  / 2;
constexpr unsigned screenMidY = screenHeight / 2;

inline const sf::Color markerColor = {156, 158, 232};

class DraggableButton : public sf::Drawable
{
	Vector2 pos;
	sf::Color color;
	Vector2 clickOffset = {};
	bool clicked = false;

public:
	DraggableButton(const Vector2& pos, sf::Color color = markerColor):
		pos(pos), color(color)
	{}

	void Click(const Vector2& clickedPos);
	void Unclick() { clicked = false; }
	void UpdatePos(const Vector2& mousePos);

	const Vector2& GetPos() const { return pos; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

const sf::CircleShape& GetPointMarker(const sf::Vector2f& pos, sf::Color color = markerColor, bool filled = false);

template<class T>
const sf::CircleShape& GetPointMarker(const sf::Vector2<T>& pos, sf::Color color = markerColor, bool filled = false)
{
	return GetPointMarker(static_cast<sf::Vector2f>(pos), color, filled);
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

template<class T, class R = int64_t>
inline sf::Vector2<R> ToWorldCoordinates(const sf::Vector2<T>& v)
{
	return {v.x - screenMidX, screenMidY - v.y};
}

inline void drawLabel(
	sf::RenderTarget& target,
	const char* string,
	const sf::Font& font,
	const sf::Vector2f& pos,
	const sf::Vector2f& offset,
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
