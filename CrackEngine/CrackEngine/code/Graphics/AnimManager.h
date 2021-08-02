#pragma once
#include "Sprite.h"
#include "Animation.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class AnimManager
{
public:
	AnimManager(Sprite* _sprite, json _characterData);
	AnimManager(Sprite* _sprite, string _characterData);
	AnimManager(Sprite* _sprite, map<Animation, int> _anims);
	AnimManager(json _characterData);
	Sprite* sprite;
	void render();
	void update();

	map<int, Animation> animList;
	Frame* currentFrame;
	Animation* currentAnim;
	int frameCount = 0;
protected:
	void init();
	void parseJson(json& _jsonData);
};

