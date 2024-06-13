#pragma once
#include "polygon.h"
#include <stdexcept>

struct SpeedZone : Polygon
{
	enum Type { offset, factor, cap, constant } type;

	double speed_value;

	double transform_speed(double speed) const
	{
		switch (type)
		{
		case offset:
			return speed + speed_value;
		case factor:
			return speed * speed_value;
		case cap:
			return std::min(speed, speed_value);
		case constant:
			return speed_value;
		}

		throw std::logic_error("Invalid speed zone");
	}
};
