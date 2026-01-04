#include "AnimManager.h"
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

glm::vec3& AnimManager::setScale(glm::vec3 _scale)
{
	sprite->setScale(_scale);
	return _scale;
}

void AnimManager::processInputs(GLFWwindow* window, InputManager& _inputs)
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
				case Frame::InputCommand::UPFORWARD:
				{
					if (sprite->getScale().x > 0)
					{
						if (_inputs.getButton(InputManager::eInputs::NORTHEAST) != true) valid = false;
					}
					else
					{
						if (_inputs.getButton(InputManager::eInputs::NORTHWEST) != true) valid = false;
					}
					break;
				}
				case Frame::InputCommand::UPBACK:
				{
					if (sprite->getScale().x > 0)
					{
						if (_inputs.getButton(InputManager::eInputs::NORTHWEST) != true) valid = false;
					}
					else
					{
						if (_inputs.getButton(InputManager::eInputs::NORTHEAST) != true) valid = false;
					}
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
				case Frame::InputCommand::DOUBLEFORWARD:
				{
					if (sprite->getScale().x > 0)
					{
						auto buffer = _inputs.getBuffer(30);
						if (buffer.size() >= 3)
						{
							auto buffer = _inputs.getBuffer(30);
							if (buffer[0].bEast != true) valid = false;
							if (buffer[1].bEast != false) valid = false;
							if (buffer[2].bEast != true) valid = false;
						}
						else
						{
							valid = false;
						}
					}
					else
					{
						auto buffer = _inputs.getBuffer(30);
						if (buffer.size() >= 3)
						{
							auto buffer = _inputs.getBuffer(30);
							if (buffer[0].bWest != true) valid = false;
							if (buffer[1].bWest != false) valid = false;
							if (buffer[2].bWest != true) valid = false;
						}
						else
						{
							valid = false;
						}
					}
					break;
				}
				case Frame::InputCommand::DOUBLEBACK:
				{
					if (sprite->getScale().x > 0)
					{
						auto buffer = _inputs.getBuffer(30);
						if (buffer.size() >= 3)
						{
							auto buffer = _inputs.getBuffer(30);
							if (buffer[0].bWest != true) valid = false;
							if (buffer[1].bWest != false) valid = false;
							if (buffer[2].bWest != true) valid = false;
						}
						else
						{
							valid = false;
						}
					}
					else
					{
						auto buffer = _inputs.getBuffer(30);
						if (buffer.size() >= 3)
						{
							auto buffer = _inputs.getBuffer(30);
							if (buffer[0].bEast != true) valid = false;
							if (buffer[1].bEast != false) valid = false;
							if (buffer[2].bEast != true) valid = false;
						}
						else
						{
							valid = false;
						}
					}
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
				/*Log::log("Change animation to following index:", Log::LOG);
				std::cout << inputAction.animChangeIndex << std::endl;*/
				changeAnimation(inputAction.animChangeIndex);
				return;
			}
		}
	}
}

void AnimManager::processActions()
{
	// update all the animations
	for (int i = 0; i < currentAnim->animActions.size(); i++)
	{
		currentAnim->animActions[i]->execute(frameCount, this);
	}
	for (int i = 0; i < currentFrame->animActions.size(); i++)
	{
		currentFrame->animActions[i]->execute(frameCount, this);
	}
}

// Define static cache
std::unordered_map<std::string, std::shared_ptr<AnimManager::CachedAnimData>> AnimManager::s_animCache;

AnimManager::AnimManager(Sprite* _sprite, std::string _characterData)
{
	sprite = _sprite;
	dataFilePath = _characterData;  // Store the file path

	// Try cache first
	auto it = s_animCache.find(_characterData);
	if (it != s_animCache.end())
	{
		animList = it->second->animList;
		paletteList = it->second->paletteList;
		characterName = it->second->characterName;
		// Initialize current anim/frame from cached defaults
		currentAnim = &animList[it->second->initialAnimIndex];
		currentFrame = &currentAnim->frameList[it->second->initialFrameIndex];
		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
		return;
	}

	// Fallback: load from file once, then cache
	std::ifstream configfile(_characterData, std::ios::out | std::ios::app | std::ios::binary);
	if (configfile.is_open())
	{
		// Proceed with output
		parseXml(_characterData.c_str());
		// Store in cache
		auto cached = std::make_shared<CachedAnimData>();
		cached->animList = animList;
		cached->paletteList = paletteList;
		cached->characterName = characterName;
		cached->initialAnimIndex = currentAnim ? currentAnim->index : 1;
		cached->initialFrameIndex = currentFrame ? currentFrame->index : 1;
		s_animCache[_characterData] = cached;
	}
	else
	{
		// Error opening file
		std::cout << "Unable to open file" << std::endl;
	}
	configfile.close();
}

AnimManager::AnimManager(std::string _characterData, glm::vec3 _position)
{
	sprite = new Sprite("");
	startPosition = sprite->setPosition(_position);
	dataFilePath = _characterData;  // Store the file path

	// Try cache
	auto it = s_animCache.find(_characterData);
	if (it != s_animCache.end())
	{
		animList = it->second->animList;
		paletteList = it->second->paletteList;
		characterName = it->second->characterName;
		currentAnim = &animList[it->second->initialAnimIndex];
		currentFrame = &currentAnim->frameList[it->second->initialFrameIndex];
		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
		return;
	}

	parseXml(_characterData.c_str());
	// Cache
	auto cached = std::make_shared<CachedAnimData>();
	cached->animList = animList;
	cached->paletteList = paletteList;
	cached->characterName = characterName;
	cached->initialAnimIndex = currentAnim ? currentAnim->index : 1;
	cached->initialFrameIndex = currentFrame ? currentFrame->index : 1;
	s_animCache[_characterData] = cached;
}

Palette AnimManager::getPalette() {
	// Get palette from sprite
	return sprite->mainPalette;
}

void AnimManager::setPalette(std::string _newColorFileName, std::string _oldColorFileName)
{
	sprite->mainPalette = Palette(_newColorFileName, _oldColorFileName);
	mainPalette = sprite->mainPalette;
}

void AnimManager::setPalette(int _index)
{
	if (_index >= 0 && _index < paletteList.size())
	{
		paletteIndex = _index;
		mainPalette = paletteList[paletteIndex];
		sprite->mainPalette = mainPalette;
	}
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
	++frameCount;
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
	// Movement
	setVelocity(acceleration, true);
	setPosition(velocity, true);
	if (checkJumpLand()) {
		return;
	}
	// Updates
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
	
	pugi::xml_node character = doc.child("character");
	// Gather character base information
	characterName = character.attribute("characterName").as_string();
	if ((character.attribute("paletteTemplate").as_string() && character.attribute("paletteTemplate").as_string() != "")
		&& (character.attribute("paletteFile").as_string() && character.attribute("paletteFile").as_string() != ""))
	{
		setPalette(character.attribute("paletteFile").as_string(), character.attribute("paletteTemplate").as_string());
		for (pugi::xml_node pal : doc.child("character").child("palettes").children("palette"))
		{
			Palette newPal(pal, character.attribute("paletteTemplate").as_string());
			paletteList.push_back(newPal);
		}
		if (paletteList.size() > 0)
		{
			setPalette(0);
		}
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
	if (_index == -1)
	{
		delete this;
	}
	currentAnim = &animList[_index];
	currentFrame = &currentAnim->frameList[1];
	sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
	sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
	sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	frameCount = -1;
	loopIndex = 0;
	if (currentAnim->keepVelocity == false)
	{
		setVelocity(glm::vec2(0, 0), false);
	}
	if (currentAnim->keepAcceleration == false)
	{
		setAcceleration(glm::vec2(0, 0));
	}
}

void AnimManager::setVelocity(glm::vec2 _vel, bool _additive)
{
	glm::vec2 vel = glm::vec2(0, 0);
	if (_additive)
	{
		vel = velocity;
	}
	vel += _vel;
	velocity = vel;
}

void AnimManager::setxVelocity(float _vel, bool _additive)
{
	if (_additive)
	{
		velocity.x += _vel;
		return;
	}
	velocity.x = _vel;
	return;
}

void AnimManager::setyVelocity(float _vel, bool _additive)
{
	if (_additive)
	{
		velocity.y += _vel;
		return;
	}
	velocity.y = _vel;
	return;
}

void AnimManager::setxAcceleration(float _accel, bool _additive)
{
	if (_additive)
	{
		acceleration.x += _accel;
		return;
	}
	acceleration.x = _accel;
	return;
}

void AnimManager::setyAcceleration(float _accel, bool _additive)
{
	if (_additive)
	{
		acceleration.y += _accel;
		return;
	}
	acceleration.y = _accel;
	return;
}

void AnimManager::setAcceleration(glm::vec2 _acc)
{
	acceleration = _acc;
}

bool AnimManager::checkJumpLand()
{
	if (velocity.y < 0 && getPosition().y < startPosition.y)
	{
		glm::vec3 floorPos = getPosition();
		floorPos.y = startPosition.y;
		setPosition(floorPos);
		setVelocity(glm::vec2(0, 0), false);
		setAcceleration(glm::vec2(0, 0));
		changeAnimation(14);
		return true;
	}
	return false;
}

void AnimManager::saveToXml(const char* filename) const {
	pugi::xml_document doc;
	
	// Create root character node
	pugi::xml_node character = doc.append_child("character");
	character.append_attribute("characterName") = characterName.c_str();
	
	// Save palette information
	if (!paletteList.empty()) {
		if (sprite && sprite->mainPalette.getTemplateFile() != "" && sprite->mainPalette.getPaletteFile() != "") {
			character.append_attribute("paletteTemplate") = sprite->mainPalette.getTemplateFile().c_str();
			character.append_attribute("paletteFile") = sprite->mainPalette.getPaletteFile().c_str();
		}
		
		pugi::xml_node palettes = character.append_child("palettes");
		for (const auto& palette : paletteList) {
			pugi::xml_node pal = palettes.append_child("palette");
			// Save palette colors using the palette's save method
			palette.saveToXml(pal);
		}
	}
	
	// Save animations
	pugi::xml_node anims = character.append_child("anims");
	for (const auto& [index, anim] : animList) {
		if (index == 0) continue;  // Skip index 0 as it's usually empty/invalid
		
		pugi::xml_node animNode = anims.append_child("anim");
		animNode.append_attribute("index") = index;
		animNode.append_attribute("name") = anim.name.c_str();
		animNode.append_attribute("fallback") = anim.fallbackindex;
		animNode.append_attribute("keepVelocity") = anim.keepVelocity;
		animNode.append_attribute("keepAcceleration") = anim.keepAcceleration;
		
		// Save frames
		pugi::xml_node frames = animNode.append_child("frames");
		for (const auto& [frameIndex, frame] : anim.frameList) {
			if (frameIndex == 0) continue;  // Skip index 0
			
			pugi::xml_node frameNode = frames.append_child("frame");
			frameNode.append_attribute("index") = frame.index;
			frameNode.append_attribute("filename") = frame.spriteFileName.c_str();
			frameNode.append_attribute("stepcount") = frame.frameCount;
			frameNode.append_attribute("xscale") = frame.xScale;
			frameNode.append_attribute("yscale") = frame.yScale;
			frameNode.append_attribute("xpos") = frame.xPos;
			frameNode.append_attribute("ypos") = frame.yPos;
			if (frame.looping) {
				frameNode.append_attribute("loop") = frame.looping;
			}
			
			// Save hitboxes
			if (frame.hitbox) {
				pugi::xml_node hitboxNode = frameNode.append_child("hitbox");
				hitboxNode.append_attribute("left") = frame.hitbox->left;
				hitboxNode.append_attribute("right") = frame.hitbox->right;
				hitboxNode.append_attribute("up") = frame.hitbox->up;
				hitboxNode.append_attribute("down") = frame.hitbox->down;
			}
			
			for (const auto& box : frame.hitboxes) {
				pugi::xml_node hitboxNode = frameNode.append_child("additionalHitbox");
				hitboxNode.append_attribute("left") = box->left;
				hitboxNode.append_attribute("right") = box->right;
				hitboxNode.append_attribute("up") = box->up;
				hitboxNode.append_attribute("down") = box->down;
			}
			
			// Save frame input actions
			if (!frame.inputActions.empty()) {
				pugi::xml_node inputActions = frameNode.append_child("inputactions");
				for (const auto& action : frame.inputActions) {
					pugi::xml_node inputAction = inputActions.append_child("inputaction");
					inputAction.append_attribute("animset") = action.animChangeIndex;
					
					// Convert input command enum to string
					std::string inputCmd;
					switch (action.inputCommand) {
						case Frame::InputCommand::FORWARD: inputCmd = "FORWARD"; break;
						case Frame::InputCommand::BACK: inputCmd = "BACK"; break;
						case Frame::InputCommand::UP: inputCmd = "UP"; break;
						case Frame::InputCommand::DOWN: inputCmd = "DOWN"; break;
						case Frame::InputCommand::UPFORWARD: inputCmd = "UPFORWARD"; break;
						case Frame::InputCommand::UPBACK: inputCmd = "UPBACK"; break;
						case Frame::InputCommand::NONE: inputCmd = "NONE"; break;
						case Frame::InputCommand::ANY: inputCmd = "ANY"; break;
						case Frame::InputCommand::DOUBLEFORWARD: inputCmd = "DOUBLEFORWARD"; break;
						case Frame::InputCommand::DOUBLEBACK: inputCmd = "DOUBLEBACK"; break;
						default: inputCmd = "NONE"; break;
					}
					inputAction.append_attribute("input") = inputCmd.c_str();
					
					// Save input string if not empty
					if (!action.inputString.empty()) {
						inputAction.append_attribute("inputString") = action.inputString.c_str();
					}
					
					// Convert input button enum to string if it's not ANY
					if (action.inputButton != Frame::InputButton::ANY) {
						std::string buttonStr;
						switch (action.inputButton) {
							case Frame::InputButton::A: buttonStr = "A"; break;
							case Frame::InputButton::B: buttonStr = "B"; break;
							case Frame::InputButton::C: buttonStr = "C"; break;
							case Frame::InputButton::X: buttonStr = "X"; break;
							case Frame::InputButton::Y: buttonStr = "Y"; break;
							case Frame::InputButton::Z: buttonStr = "Z"; break;
							default: break;
						}
						if (!buttonStr.empty()) {
							inputAction.append_attribute("button") = buttonStr.c_str();
						}
					}
				}
			}
		}
		
		// Save animation input actions
		if (!anim.inputActions.empty()) {
			pugi::xml_node inputActions = animNode.append_child("inputactions");
			for (const auto& action : anim.inputActions) {
				pugi::xml_node inputAction = inputActions.append_child("inputaction");
				inputAction.append_attribute("animset") = action.animChangeIndex;
				
				// Convert input command enum to string
				std::string inputCmd;
				switch (action.inputCommand) {
					case Frame::InputCommand::FORWARD: inputCmd = "FORWARD"; break;
					case Frame::InputCommand::BACK: inputCmd = "BACK"; break;
					case Frame::InputCommand::UP: inputCmd = "UP"; break;
					case Frame::InputCommand::DOWN: inputCmd = "DOWN"; break;
					case Frame::InputCommand::UPFORWARD: inputCmd = "UPFORWARD"; break;
					case Frame::InputCommand::UPBACK: inputCmd = "UPBACK"; break;
					case Frame::InputCommand::NONE: inputCmd = "NONE"; break;
					case Frame::InputCommand::ANY: inputCmd = "ANY"; break;
					case Frame::InputCommand::DOUBLEFORWARD: inputCmd = "DOUBLEFORWARD"; break;
					case Frame::InputCommand::DOUBLEBACK: inputCmd = "DOUBLEBACK"; break;
					default: inputCmd = "NONE"; break;
				}
				inputAction.append_attribute("input") = inputCmd.c_str();
				
				// Save input string if not empty
				if (!action.inputString.empty()) {
					inputAction.append_attribute("inputString") = action.inputString.c_str();
				}
				
				// Convert input button enum to string if it's not ANY
				if (action.inputButton != Frame::InputButton::ANY) {
					std::string buttonStr;
					switch (action.inputButton) {
						case Frame::InputButton::A: buttonStr = "A"; break;
						case Frame::InputButton::B: buttonStr = "B"; break;
						case Frame::InputButton::C: buttonStr = "C"; break;
						case Frame::InputButton::X: buttonStr = "X"; break;
						case Frame::InputButton::Y: buttonStr = "Y"; break;
						case Frame::InputButton::Z: buttonStr = "Z"; break;
						default: break;
					}
					if (!buttonStr.empty()) {
						inputAction.append_attribute("button") = buttonStr.c_str();
					}
				}
			}
		}
	}
	
	// Save the document
	bool saveSucceeded = doc.save_file(filename);
	if (!saveSucceeded) {
		std::cout << "Failed to save character data to " << filename << std::endl;
	}
}

bool AnimManager::tryChangeAnimationByString(const std::string& inputString) {
	// First check current frame's input actions
	for (const auto& action : currentFrame->inputActions) {
		if (!action.inputString.empty() && action.inputString == inputString) {
			changeAnimation(action.animChangeIndex);
			return true;
		}
	}

	// Then check current animation's input actions
	for (const auto& action : currentAnim->inputActions) {
		if (!action.inputString.empty() && action.inputString == inputString) {
			changeAnimation(action.animChangeIndex);
			return true;
		}
	}

	return false;  // No matching input string found
}
