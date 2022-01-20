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
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"DOWN")
			{
				command = Frame::InputCommand::DOWN;
			}
		}
		InputAction tempAction;
		tempAction.animChangeIndex = animIndex;
		tempAction.inputCommand = command;
		inputActions.push_back(tempAction);
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
