#include "Frame.h"

unsigned int loadTexture(char const* path, int* _width, int* _height);

Frame::Frame(json _frameData)
{
	spriteFileName = "";
	xScale = 1;
	yScale = 1;
	xPos = 0;
	yPos = 0;
	spriteImage = 0;
	spriteWidth = 0;
	spriteHeight = 0;
	frameCount = 1;
	index = -1;

	if (_frameData["index"] != NULL)
		index = _frameData["index"];
	if (_frameData["filename"] != NULL)
		spriteFileName = (std::string)_frameData["filename"];
	if (_frameData["framecount"] != NULL)
		frameCount = _frameData["frameCount"];
	if (_frameData["xscale"] != NULL)
		xScale = _frameData["xscale"];
}

Frame::Frame(pugi::xml_node _frameData)
{
	spriteFileName = "";
	xScale = 1;
	yScale = 1;
	xPos = 0;
	yPos = 0;
	spriteImage = 0;
	spriteWidth = 0;
	spriteHeight = 0;
	frameCount = 1;
	index = 0;

	if (_frameData.attribute("xscale").as_int() != NULL)
	{
		xScale = _frameData.attribute("xscale").as_int();
	}
	if (_frameData.attribute("yscale").as_int() != NULL)
	{
		yScale = _frameData.attribute("yscale").as_int();
	}
	if (_frameData.attribute("xpos").as_int() != NULL)
	{
		xPos = _frameData.attribute("xpos").as_int();
	}
	if (_frameData.attribute("ypos").as_int() != NULL)
	{
		yPos = _frameData.attribute("ypos").as_int();
	}
	if (_frameData.attribute("filename").as_string() != NULL)
	{
		spriteFileName = _frameData.attribute("filename").as_string();
	}
	else
	{
		return;
	}
	if (_frameData.attribute("stepcount").as_int() != NULL)
	{
		frameCount = _frameData.attribute("stepcount").as_int();
	}
	else
	{
		return;
	}
	if (_frameData.attribute("index").as_int() != NULL)
	{
		index = _frameData.attribute("index").as_int();
	}
	if (_frameData.attribute("loop").as_bool() != NULL)
	{
		looping = _frameData.attribute("loop").as_bool();
	}
	for (pugi::xml_node action : _frameData.child("inputactions").children("inputaction"))
	{
		int animIndex = -1;
		Frame::InputCommand command = Frame::InputCommand::EMPTY;
		Frame::InputButton button = Frame::InputButton::EMPTY;
		if (action.attribute("animset").as_int() != NULL) animIndex = action.attribute("animset").as_int();
		if (action.attribute("input").as_string() != NULL)
		{
			if ((std::string)(action.attribute("input").as_string()) == (std::string)"FORWARD")
			{
				command = Frame::InputCommand::FORWARD;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"NONE")
			{
				command = Frame::InputCommand::NONE;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"BACK")
			{
				command = Frame::InputCommand::BACK;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"UP")
			{
				command = Frame::InputCommand::UP;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"DOWN")
			{
				command = Frame::InputCommand::DOWN;
			}
		}
		if (action.attribute("button").as_string() != NULL)
		{
			if ((std::string)(action.attribute("button").as_string()) == (std::string)"A")
			{
				button = Frame::InputButton::A;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"B")
			{
				button = Frame::InputButton::B;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"C")
			{
				button = Frame::InputButton::C;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"X")
			{
				button = Frame::InputButton::X;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"Y")
			{
				button = Frame::InputButton::Y;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"Z")
			{
				button = Frame::InputButton::Z;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"ANY")
			{
				button = Frame::InputButton::ANY;
			}
			else if ((std::string)(action.attribute("button").as_string()) == (std::string)"NONE")
			{
				button = Frame::InputButton::NONE;
			}
		}
		InputAction tempAction;
		tempAction.animChangeIndex = animIndex;
		tempAction.inputCommand = command;
		tempAction.inputButton = button;
		inputActions.push_back(tempAction);
	}
	for (pugi::xml_node action : _frameData.child("frameactions").children("frameaction"))
	{
		float xDelta = 0;
		if (action.attribute("xdelta").as_float() != NULL) xDelta = action.attribute("xdelta").as_float();
		float yDelta = 0;
		if (action.attribute("ydelta").as_float() != NULL) yDelta = action.attribute("ydelta").as_float();
		float xVelocity = 0;
		if (action.attribute("xvelocity").as_float() != NULL) xVelocity = action.attribute("xvelocity").as_float();
		float yVelocity = 0;
		if (action.attribute("yvelocity").as_float() != NULL) yVelocity = action.attribute("yvelocity").as_float();
		float xAcceleration = 0;
		if (action.attribute("xacceleration").as_float() != NULL) xAcceleration = action.attribute("xacceleration").as_float();
		float yAcceleration = 0;
		if (action.attribute("yacceleration").as_float() != NULL) yAcceleration = action.attribute("yacceleration").as_float();
		FrameAction tempAction;
		tempAction.xDelta = xDelta;
		tempAction.yDelta = yDelta;
		tempAction.xVelocity = xVelocity;
		tempAction.yVelocity = yVelocity;
		tempAction.xAcceleration = xAcceleration;
		tempAction.yAcceleration = yAcceleration;
		frameActions.push_back(tempAction);
	}

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	spriteImage = loadTexture(spriteFileName.c_str(), &spriteWidth, &spriteHeight);
}

Frame::Frame(int _xScale, int _yScale, int _xPos, int _yPos, std::string _fileName, int _frameCount, int _index)
{
	xScale = _xScale;
	yScale = _yScale;
	xPos = _xPos;
	yPos = _yPos;
	spriteFileName = _fileName;
	frameCount = _frameCount;
	index = _index;

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	spriteImage = loadTexture(spriteFileName.c_str(), &spriteWidth, &spriteHeight);
}

unsigned int loadTexture(char const* path, int* _width, int* _height)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int nrComponents;
	unsigned char* data = stbi_load(path, _width, _height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, *_width, *_height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
