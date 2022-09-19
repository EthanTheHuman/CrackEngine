#include "AnimManager.h"

glm::vec3& AnimManager::setScale(glm::vec3 _scale)
{
	sprite->setScale(_scale);
	return _scale;
}

void AnimManager::processInputs(GLFWwindow* window, InputManager _inputs)
{
	std::vector<Frame::InputAction> actions;
	actions.reserve(currentFrame->inputActions.size() + currentAnim->inputActions.size());
	actions.insert(actions.end(), currentFrame->inputActions.begin(), currentFrame->inputActions.end());
	actions.insert(actions.end(), currentAnim->inputActions.begin(), currentAnim->inputActions.end());

	for (int i = 0; i < actions.size(); i++)
	{
		Frame::InputAction& inputAction = actions[i];
		if (inputAction.animChangeIndex != NULL)
		{
			bool valid = true;
			switch (inputAction.inputCommand)
			{
				case Frame::InputCommand::NONE:
				{
					bool h = false;
					if (_inputs.getButton(InputManager::eInputs::EAST) == true && _inputs.getButton(InputManager::eInputs::WEST) == true)
					{
						h = false;
					}
					else if (_inputs.getButton(InputManager::eInputs::EAST) == true || _inputs.getButton(InputManager::eInputs::WEST) == true)
					{
						h = true;
					}
					
					bool v = false;
					if (_inputs.getButton(InputManager::eInputs::NORTH) == true && _inputs.getButton(InputManager::eInputs::SOUTH) == true)
					{
						v = false;
					}
					else if (_inputs.getButton(InputManager::eInputs::NORTH) == true || _inputs.getButton(InputManager::eInputs::SOUTH) == true)
					{
						v = true;
					}

					if (h == true || v == true)
					{
						valid = false;
					}

					break;
				}
				case Frame::InputCommand::FORWARD:
				{
					if (sprite->getScale().x > 0)
					{
						if (_inputs.getButton(InputManager::eInputs::EAST) != true) valid = false;
						if (_inputs.getButton(InputManager::eInputs::WEST) == true) valid = false;
					}
					else
					{
						if (_inputs.getButton(InputManager::eInputs::WEST) != true) valid = false;
						if (_inputs.getButton(InputManager::eInputs::EAST) == true) valid = false;
					}
					break;
				}
				case Frame::InputCommand::BACK:
				{
					if (sprite->getScale().x > 0)
					{
						if (_inputs.getButton(InputManager::eInputs::WEST) != true) valid = false;
						if (_inputs.getButton(InputManager::eInputs::EAST) == true) valid = false;
					}
					else
					{
						if (_inputs.getButton(InputManager::eInputs::EAST) != true) valid = false;
						if (_inputs.getButton(InputManager::eInputs::WEST) == true) valid = false;
					}
					break;
				}
				case Frame::InputCommand::UP:
				{
					if (_inputs.getButton(InputManager::eInputs::NORTH) != true
						&& _inputs.getButton(InputManager::eInputs::NORTHWEST) != true
						&& _inputs.getButton(InputManager::eInputs::NORTHEAST) != true) valid = false;
					break;
				}
				case Frame::InputCommand::DOWN:
				{
					if ((_inputs.getButton(InputManager::eInputs::SOUTH) != true)
					&& (_inputs.getButton(InputManager::eInputs::SOUTHWEST) != true)
					&& (_inputs.getButton(InputManager::eInputs::SOUTHEAST) != true)) valid = false;
					break;
				}
				case Frame::InputCommand::ANY:
				{
					break;
				}
				case Frame::InputCommand::EMPTY:
				{
					break;
				}
			}
			switch (inputAction.inputButton)
			{
				case Frame::InputButton::NONE:
				{
					if (_inputs.getButton(InputManager::eInputs::A) == true)
						valid = false;
					if (_inputs.getButton(InputManager::eInputs::B) == true)
						valid = false;
					if (_inputs.getButton(InputManager::eInputs::C) == true)
						valid = false;
					if (_inputs.getButton(InputManager::eInputs::X) == true)
						valid = false;
					if (_inputs.getButton(InputManager::eInputs::Y) == true)
						valid = false;
					if (_inputs.getButton(InputManager::eInputs::Z) == true)
						valid = false;
					
					break;
				}
				case Frame::InputButton::A:
				{
					if (_inputs.getButton(InputManager::eInputs::A) == false)
						valid = false;
					break;
				}
				case Frame::InputButton::B:
				{
					if (_inputs.getButton(InputManager::eInputs::B) == false)
						valid = false;
					break;
				}
				case Frame::InputButton::C:
				{
					if (_inputs.getButton(InputManager::eInputs::C) == false)
						valid = false;
					break;
				}
				case Frame::InputButton::X:
				{
					if (_inputs.getButton(InputManager::eInputs::X) == false)
						valid = false;
					break;
				}
				case Frame::InputButton::Y:
				{
					if (_inputs.getButton(InputManager::eInputs::Y) == false)
						valid = false;
					break;
				}
				case Frame::InputButton::Z:
				{
					if (_inputs.getButton(InputManager::eInputs::Z) == false)
						valid = false;
					break;
				}
				case Frame::InputButton::ANY:
				{
					break;
				}
				case Frame::InputButton::EMPTY:
				{
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

void AnimManager::processActions()
{
	for (int i = 0; i < currentAnim->frameActions.size(); i++)
	{
		processAction(currentAnim->frameActions[i]);
	};

	for (int i = 0; i < currentFrame->frameActions.size(); i++)
	{
		processAction(currentFrame->frameActions[i]);
	};
}

void AnimManager::processAction(Frame::FrameAction _action)
{
	// Moving sideways
	if (_action.xDelta != 0)
	{
		setPosition(glm::vec2(_action.xDelta, 0), true);
	};
	if (_action.yDelta != 0)
	{
		setPosition(glm::vec2(0, _action.yDelta), true);
	}
	if (_action.xVelocity != 0)
	{
		setVelocity(glm::vec2(_action.xVelocity, velocity.y), false);
	}
	if (_action.yVelocity != 0)
	{
		setVelocity(glm::vec2(velocity.x, _action.yVelocity), false);
	}
	if (_action.xAcceleration != 0)
	{
		setAcceleration(glm::vec2(_action.xAcceleration, acceleration.y));
	}
	if (_action.yAcceleration != 0)
	{
		setAcceleration(glm::vec2(acceleration.x, _action.yAcceleration));
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

glm::vec3& AnimManager::setPosition(glm::vec3 _pos, bool _additive)
{
	glm::vec3 pos = glm::vec3(0, 0, 0);
	if (_additive)
	{
		pos = getPosition();
	}
	pos.x += _pos.x;
	pos.y += _pos.y;
	pos.z += _pos.z;
	return setPosition(pos);
}

glm::vec3& AnimManager::setPosition(glm::vec2 _pos, bool _additive)
{
	glm::vec3 pos = glm::vec3(0, 0, 0);
	if (_additive)
	{
		pos = getPosition();
	}
	pos.x += _pos.x;
	pos.y += _pos.y;
	return setPosition(pos);
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
		if (currentFrame->looping == true) loopIndex = currentFrame->index;
		if (currentAnim->frameList.find(newIndex) == currentAnim->frameList.end() && currentFrame->frameCount != -1) {
			if (loopIndex != 0)
			{
				currentFrame = &currentAnim->frameList[loopIndex];
				loopIndex = 0;
			}
			else
			{
				changeAnimation(currentAnim->fallbackindex);
				//currentFrame = &currentAnim->frameList[1];
			}
			loopIndex = 0;
		}
		else {
			currentFrame = &currentAnim->frameList[newIndex];
		}

		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	}
	setVelocity(acceleration, true);
	std::cout << "acc: " << acceleration.y << std::endl;
	std::cout << "vel: " << velocity.y << std::endl;
	setPosition(velocity, true);
	sprite->update();
	processActions();
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
	currentFrame = &currentAnim->frameList[1];
	sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
	sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
	sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	frameCount = 0;
	loopIndex = 0;
	setVelocity(glm::vec2(0, 0), false);
	setAcceleration(glm::vec2(0, 0));
}

void AnimManager::setVelocity(glm::vec2 _vel, bool _additive = false)
{
	glm::vec2 vel = glm::vec2(0, 0);
	if (_additive)
	{
		vel = velocity;
	}
	vel += _vel;
	velocity = vel;
}

void AnimManager::setAcceleration(glm::vec2 _acc)
{
	acceleration = _acc;
}
