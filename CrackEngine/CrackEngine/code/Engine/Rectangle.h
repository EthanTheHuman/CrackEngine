#pragma once
class Rectangle
{
public:
	Rectangle();
	~Rectangle();
	Rectangle(float _left, float _right, float _up, float _down, bool _isInt = true);
	
	bool Intersects(Rectangle _other);
	bool Intersects(float x, float y);
	float left = 0;
	float right = 0;
	float up = 0;
	float down = 0;
};

