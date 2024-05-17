#pragma once
#include <SFML/Graphics.hpp>
#include "math.h"

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
