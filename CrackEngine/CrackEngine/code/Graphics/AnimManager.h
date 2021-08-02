#pragma once
#include "Sprite.h"
#include "Animation.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class AnimManager :
	public GameObject
{
public:
	void init() override;
	void update() override;
	void render() override;
	glm::vec3 getPosition() override;
	glm::vec3& setPosition(glm::vec3 _pos) override;
	//glm::vec3 getRotation() override;
	//glm::vec3& setRotation(glm::vec3 _rot) override;
	//glm::vec3 getScale() override;
	glm::vec3& setScale(glm::vec3 _scale) override;

	AnimManager(Sprite* _sprite, json _characterData);
	AnimManager(Sprite* _sprite, string _characterData);
	AnimManager(Sprite* _sprite, map<Animation, int> _anims);
	AnimManager(string _characterData);
	Sprite* sprite;

	map<int, Animation> animList;
	Frame* currentFrame;
	Animation* currentAnim;
	int frameCount = 0;
protected:
	void parseJson(json& _jsonData);
};

