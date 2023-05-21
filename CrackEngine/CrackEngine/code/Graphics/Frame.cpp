#include "Frame.h"
// anim actions
#include "../Gameplay/Anim Actions/Act_SetXVel.h"
#include "../Gameplay/Anim Actions/Act_SetYVel.h"
#include "../Gameplay/Anim Actions/Act_AddXVel.h"
#include "../Gameplay/Anim Actions/Act_AddYVel.h"
#include "../Gameplay/Anim Actions/Act_SetXAccel.h"
#include "../Gameplay/Anim Actions/Act_SetYAccel.h"
#include "../Gameplay/Anim Actions/Act_AddXAccel.h"
#include "../Gameplay/Anim Actions/Act_AddYAccel.h"
#include "../Gameplay/Anim Actions/Act_MoveDelta.h"
#include "../Gameplay/Anim Actions/Act_PlaySound.h"

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
	inputActions = readInputActionsFromXML(_frameData);
	animActions = readAnimActionsFromXML(_frameData);

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

std::vector<Frame::InputAction> Frame::readInputActionsFromXML(pugi::xml_node _data)
{
	std::vector<InputAction> returnInputActions;
	for (pugi::xml_node action : _data.child("inputactions").children("inputaction"))
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
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"UPFORWARD")
			{
				command = Frame::InputCommand::UPFORWARD;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"UPBACK")
			{
				command = Frame::InputCommand::UPBACK;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"DOUBLEFORWARD")
			{
				command = Frame::InputCommand::DOUBLEFORWARD;
			}
			else if ((std::string)(action.attribute("input").as_string()) == (std::string)"DOUBLEBACK")
			{
				command = Frame::InputCommand::DOUBLEBACK;
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
		returnInputActions.push_back(tempAction);
	}
	return returnInputActions;
}

std::vector<AnimAction*> Frame::readAnimActionsFromXML(pugi::xml_node _xml)
{
	std::vector<AnimAction*> returnActions;
	for (pugi::xml_node action : _xml.child("animactions").children("animaction"))
	{
		AnimAction* newAction = readAnimActionFromXML(action);
		if (newAction != nullptr)
		{
			returnActions.push_back(newAction);
		}
	}
	return returnActions;
}

AnimAction* Frame::readAnimActionFromXML(pugi::xml_node _xml)
{
	if (_xml.attribute("type").as_string() != NULL)
	{
		std::string type = _xml.attribute("type").as_string();
		// TODO: All the different animaction types
		if (type == "setxvel" || type == "setXVel" || type == "SetXVel" || type == "setxVel")
		{
			return new Act_SetXVel(_xml);
		}
		if (type == "setyvel" || type == "setYVel" || type == "SetYVel" || type == "setyVel")
		{
			return new Act_SetYVel(_xml);
		}
		if (type == "addxvel" || type == "addXVel" || type == "AddXVel" || type == "addxVel")
		{
			return new Act_AddXVel(_xml);
		}
		if (type == "addyvel" || type == "addYVel" || type == "AddYVel" || type == "addyVel")
		{
			return new Act_AddYVel(_xml);
		}
		if (type == "setxaccel" || type == "setXAccel" || type == "SetXAccel" || type == "setxAccel")
		{
			return new Act_SetXAccel(_xml);
		}
		if (type == "setyaccel" || type == "setYAccel" || type == "SetYAccel" || type == "setyAccel")
		{
			return new Act_SetYAccel(_xml);
		}
		if (type == "addxaccel" || type == "addXAccel" || type == "AddXAccel" || type == "addxAccel")
		{
			return new Act_AddXAccel(_xml);
		}
		if (type == "addyaccel" || type == "addYAccel" || type == "AddYAccel" || type == "addyAccel")
		{
			return new Act_AddYAccel(_xml);
		}
		if (type == "moveDelta" || type == "MoveDelta" || type == "movedelta")
		{
			return new Act_MoveDelta(_xml);
		}
		if (type == "playSound" || type == "sound" || type == "PlaySound" || type == "playsound" || type == "Sound")
		{
			return new Act_PlaySound(_xml);
		}
	}
	return nullptr;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
