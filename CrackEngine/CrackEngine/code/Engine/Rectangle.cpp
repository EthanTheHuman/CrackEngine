#include "./Rectangle.h"

Rectangle::Rectangle()
{
	left = -1;
	right = 1;
	up = 1;
	down = -1;
}

Rectangle::~Rectangle()
{
}

Rectangle::Rectangle(float _left, float _right, float _up, float _down, bool _isInt)
{
	left = _left;
	right = _right;
	up = _up;
	down = _down;
	if (_isInt)
	{
		left = (int)_left;
		right = (int)_right;
		up = (int)_up;
		down = (int)_down;
	}
}

bool Rectangle::Intersects(Rectangle _other)
{
	if (left > _other.right || right < _other.left || up < _other.down || down > _other.up)
	{
		return false;
	}
	return true;
}

bool Rectangle::Intersects(float x, float y)
{
	if (x > right || x < left || y < down || y > up)
	{
		return false;
	}
	return true;
}
