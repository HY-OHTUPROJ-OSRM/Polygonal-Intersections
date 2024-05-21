#include "gui.h"

constexpr int pointMarkerRadius = 10;

void DraggableButton::Click(sf::Vector2i clickedPos)
{
	const Vector2 offset = pos - ToWorldCoordinates(clickedPos);

	if (Dot(offset, offset) <= pointMarkerRadius * pointMarkerRadius)
	{
		clickOffset = offset;
		clicked = true;
	}
}

void DraggableButton::UpdatePos(sf::Vector2i mousePos)
{
	if (!clicked) return;

	pos = ToWorldCoordinates(mousePos) + clickOffset;

	const double w = screenMidX - pointMarkerRadius;
	const double h = screenMidY - pointMarkerRadius;

	pos.x = std::clamp<double>(pos.x, -w, w);
	pos.y = std::clamp<double>(pos.y, -h, h);
}

void DraggableButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(GetPointMarker(pos, color, clicked), states);
}

const sf::CircleShape& GetPointMarker(sf::Vector2f screenPos, sf::Color color, bool filled)
{
	struct Marker : sf::CircleShape
	{
		Marker() : CircleShape{pointMarkerRadius}
		{
			setOrigin(pointMarkerRadius, pointMarkerRadius);
			setOutlineThickness(1);
		}
	}
	static marker;

	marker.setPosition(screenPos);
	marker.setOutlineColor(color);
	marker.setFillColor(filled ? color : sf::Color::Transparent);

	return marker;
}
