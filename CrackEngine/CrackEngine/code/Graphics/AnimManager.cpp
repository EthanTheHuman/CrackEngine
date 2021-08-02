#include "AnimManager.h"

AnimManager::AnimManager(Sprite* _sprite, json _characterData)
{
	sprite = _sprite;
	parseJson(_characterData);
}

AnimManager::AnimManager(Sprite* _sprite, string _characterData)
{
	sprite = _sprite;

	ifstream configfile(_characterData, ios::out | ios::app | ios::binary);
	if (configfile.is_open())
	{
		// Proceed with output
		json jsonData;
		configfile >> jsonData;

		parseJson(jsonData);
		currentAnim = &animList[0];
		currentFrame = &currentAnim->frameList[0];
		sprite->setImage(currentFrame->spriteImage, currentFrame->spriteWidth, currentFrame->spriteHeight);
		sprite->frameScale = glm::vec3(currentFrame->xScale, currentFrame->yScale, 1);
		sprite->framePos = glm::vec3(currentFrame->xPos, currentFrame->yPos, 0);
	}
	else
	{
		// Error opening file
		cout << "Unable to open file" << endl;
	}
	configfile.close();
}

void AnimManager::render()
{
	sprite->render();
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
}

void AnimManager::parseJson(json& _jsonData)
{
	for (const auto& anim : _jsonData["anims"])
	{
		if (anim.find("frames") != anim.end())
		{
			// frame count
			map<int, Frame> frameList;
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
				string spriteFileName = "";
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
