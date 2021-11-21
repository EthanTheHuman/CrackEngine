#include "AnimManager.h"

glm::vec3& AnimManager::setScale(glm::vec3 _scale)
{
	sprite->setScale(_scale);
	return _scale;
}

AnimManager::AnimManager(Sprite* _sprite, json _characterData)
{
	sprite = _sprite;
	parseJson(_characterData);
}

AnimManager::AnimManager(Sprite* _sprite, std::string _characterData)
{
	sprite = _sprite;

	std::ifstream configfile(_characterData, std::ios::out | std::ios::app | std::ios::binary);
	if (configfile.is_open())
	{
		// Proceed with output
		json jsonData;
		configfile >> jsonData;

		parseJson(jsonData);
		currentAnim = &animList[0];
		currentFrame = &currentAnim->frameList[0];
		mainPalette = Palette();
		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
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
	//std::ifstream configfile(_characterData, std::ios::out | std::ios::app | std::ios::binary);
	//if (configfile.is_open())
	//{
	//	// Proceed with output
	//	json jsonData;
	//	configfile >> jsonData;

	//	parseJson(jsonData);
	//	currentAnim = &animList[0];
	//	currentFrame = &currentAnim->frameList[0];
	//	sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
	//	sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
	//	sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	//}
	//else
	//{
	//	// Error opening file
	//	std::cout << "Unable to open file" << std::endl;
	//}
	//configfile.close();
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
	if (frameCount > currentFrame->frameCount)
	{
		int newIndex = currentFrame->index + 1;
		if (currentAnim->frameList.find(newIndex) == currentAnim->frameList.end()) {
			currentFrame = &currentAnim->frameList[0];
		}
		else {
			currentFrame = &currentAnim->frameList[newIndex];
		}
		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
		frameCount = 0;
	}
	sprite->update();
}

void AnimManager::parseJson(json& _jsonData)
{
	for (const auto& anim : _jsonData["anims"])
	{
		if (anim.find("frames") != anim.end())
		{
			// frame count
			std::map<int, Frame> frameList;
			for (const auto& frame : anim["frames"])
			{
				int xScale = 1;
				if (frame.find("xscale") != frame.end() && frame["xscale"].is_number_integer())
					xScale = frame["xscale"];
				int yScale = 1;
				if (frame.find("yscale") != frame.end() && frame["yscale"].is_number_integer())
					yScale = frame["yscale"];
				int xPos = 0;
				if (frame.find("xpos") != frame.end() && frame["xpos"].is_number_integer())
					xPos = frame["xpos"];
				int yPos = 0;
				if (frame.find("ypos") != frame.end() && frame["ypos"].is_number_integer())
					yPos = frame["ypos"];
				std::string spriteFileName = "";
				if (frame.find("filename") != frame.end() && frame["filename"].is_string())
					spriteFileName = frame["filename"];
				int spriteframeCount = 1;
				if (frame.find("framecount") != frame.end() && frame["framecount"].is_number_integer())
					spriteframeCount = frame["framecount"];
				int index = 0;
				if (frame.find("index") != frame.end() && frame["index"].is_number_integer())
					index = frame["index"];

				Frame newFrame(xScale, yScale, xPos, yPos, spriteFileName, spriteframeCount, index);
				frameList.emplace(newFrame.index, newFrame);
			}
			Animation newAnim(frameList, anim["index"]);
			animList[newAnim.index] = newAnim;
		}
	}
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
		// frame count
		std::map<int, Frame> frameList;
		for (pugi::xml_node frame : anim.child("frames").children("frame"))
		{
			Frame newFrame(frame);
			frameList.emplace(newFrame.index, newFrame);
		}
		Animation newAnim(frameList, anim.attribute("index").as_int());
		animList[newAnim.index] = newAnim;
	}

	currentAnim = &animList[0];
	currentFrame = &currentAnim->frameList[0];
	sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
	sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
	sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
}
