#pragma once
#include "../../includes/json.hpp"
#include "../../shader_m.h"
#include "../../stb_image.h"
#include <string>
using json = nlohmann::json;

class Frame
{
public:
	Frame(json _frameData);
	Frame(int _xScale = 1, int _yScale = 1, int _xPos = 0, int _yPos = 0, std::string _fileName = "", int _frameCount = 1, int _index = 0);
	int xScale;
	int yScale;
	int xPos;
	int yPos;
	unsigned int spriteImage;
	int spriteWidth, spriteHeight;
	int frameCount;
	int index;

protected:
	std::string spriteFileName;
private:
	// texture coordinates
	// texture width and height
};