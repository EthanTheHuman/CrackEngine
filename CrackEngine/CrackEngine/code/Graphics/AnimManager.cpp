#include "AnimManager.h"

glm::vec3& AnimManager::setScale(glm::vec3 _scale)
{
	sprite->setScale(_scale);
	return _scale;
}

void AnimManager::processInputs(GLFWwindow* window, InputManager _inputs)
{
	for (int i = 0; i < currentFrame->inputActions.size(); i++)
	{
		Frame::InputAction& inputAction = currentFrame->inputActions[i];
		if (inputAction.animChangeIndex != NULL)
		{
			bool valid = false;
			switch (inputAction.inputCommand)
			{
				case Frame::InputCommand::FORWARD:
				{
					valid = true;
					if (sprite->getScale().x > 0)
					{
						if (_inputs.getButton(InputManager::eInputs::EAST) != true) valid = false;
					}
					else
					{
						if (_inputs.getButton(InputManager::eInputs::WEST) != true) valid = false;
					}
					break;
				}
				case Frame::InputCommand::BACK:
				{
					valid = true;
					if (sprite->getScale().x > 0)
					{
						if (_inputs.getButton(InputManager::eInputs::WEST) != true) valid = false;
					}
					else
					{
						if (_inputs.getButton(InputManager::eInputs::EAST) != true) valid = false;
					}
					break;
				}
				case Frame::InputCommand::UP:
				{
					valid = true;
					if (_inputs.getButton(InputManager::eInputs::NORTH) != true) valid = false;
					if (_inputs.getButton(InputManager::eInputs::NORTHWEST) != true) valid = false;
					if (_inputs.getButton(InputManager::eInputs::NORTHEAST) != true) valid = false;
					break;
				}
				case Frame::InputCommand::DOWN:
				{
					valid = true;
					if (_inputs.getButton(InputManager::eInputs::SOUTH) != true) valid = false;
					if (_inputs.getButton(InputManager::eInputs::SOUTHWEST) != true) valid = false;
					if (_inputs.getButton(InputManager::eInputs::SOUTHEAST) != true) valid = false;
					break;
				}
				case Frame::InputCommand::NONE:
				{
					valid = true;
					if (_inputs.getButton(InputManager::eInputs::EAST) == true || _inputs.getButton(InputManager::eInputs::WEST) == true
						|| _inputs.getButton(InputManager::eInputs::NORTH) == true || _inputs.getButton(InputManager::eInputs::SOUTH) == true) valid = false;
					else if (_inputs.getButton(InputManager::eInputs::EAST) == true && _inputs.getButton(InputManager::eInputs::WEST) == true)
					{
						if (_inputs.getButton(InputManager::eInputs::NORTH) == true || _inputs.getButton(InputManager::eInputs::SOUTH) == true) valid = false;
					}
					else if (_inputs.getButton(InputManager::eInputs::NORTH) == true && _inputs.getButton(InputManager::eInputs::SOUTH) == true)
					{
						if (_inputs.getButton(InputManager::eInputs::EAST) == true || _inputs.getButton(InputManager::eInputs::WEST) == true) valid = false;
					}
					break;
				}
				case Frame::InputCommand::EMPTY:
				{
					valid = false;
					break;
				}
			}
			if (valid == true)
			{
				Log::log("Change animation", Log::IMPORTANT);
				changeAnimation(inputAction.animChangeIndex);
			}
		}
	}
}

AnimManager::AnimManager(Sprite* _sprite, std::string _characterData)
{
	sprite = _sprite;

	std::ifstream configfile(_characterData, std::ios::out | std::ios::app | std::ios::binary);
	if (configfile.is_open())
	{
		// Proceed with output

		parseXml(_characterData.c_str());
		mainPalette = Palette();
	}
	else
	{
		// Error opening file
		std::cout << "Unable to open file" << std::endl;
	}
	configfile.close();
}

AnimManager::AnimManager(std::string _characterData)
{
	sprite = new Sprite("");
	sprite->setPosition(glm::vec3(60.f, 20.f, 0.f));

	parseXml(_characterData.c_str());
}

void AnimManager::setPalette(std::string _newColorFileName, std::string _oldColorFileName)
{
	sprite->mainPalette = Palette(_newColorFileName, _oldColorFileName);
}

void AnimManager::render()
{
	sprite->render();
}

glm::vec3 AnimManager::getPosition()
{
	return sprite->getPosition();
}

glm::vec3& AnimManager::setPosition(glm::vec3 _pos)
{
	sprite->setPosition(_pos);
	return _pos;
}

void AnimManager::init()
{
}

void AnimManager::update()
{
	frameCount++;
	if (frameCount >= currentFrame->frameCount && currentFrame->frameCount != -1)
	{
		frameCount = 0;
		int newIndex = currentFrame->index + 1;
		if (currentAnim->frameList.find(newIndex) == currentAnim->frameList.end() && currentFrame->frameCount) {
			currentFrame = &currentAnim->frameList[1];
		}
		else {
			currentFrame = &currentAnim->frameList[newIndex];
		}
		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	}
	sprite->update();
}

void AnimManager::parseXml(const char* _filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(_filename);
	if (!result)
	{
		std::cout << "no file found at " << _filename << std::endl;
		return;
	}
	else
	{
		std::cout << "character anim file loaded at " << _filename << std::endl;
	}

	for (pugi::xml_node anim : doc.child("character").child("anims").children("anim"))
	{
		Animation newAnim(anim);
		animList[newAnim.index] = newAnim;
	}

	currentAnim = &animList[1];
	currentFrame = &currentAnim->frameList[1];
	sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
	sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
	sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
}

void AnimManager::changeAnimation(int _index)
{
	currentAnim = &animList[_index];
	currentFrame = &currentAnim->frameList[0];
	sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
	sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
	sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	frameCount = 0;
}
