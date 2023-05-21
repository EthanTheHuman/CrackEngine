#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "Palette.h"
#include "../Input/InputManager.h"
#include "../Log.h"
#include "../../includes/xml/pugixml.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>

// forward declare AnimAction classes
class AnimAction;
class Act_SetXVel;

class AnimManager :
	public GameObject
{
public:
	void init() override;
	void update() override;
	void render() override;
	glm::vec3 getPosition() override;
	glm::vec3& setPosition(glm::vec3 _pos) override;
	glm::vec3& setPosition(glm::vec3 _pos, bool _additive);
	glm::vec3& setPosition(glm::vec2 _pos, bool _additive);
	//glm::vec3 getRotation() override;
	//glm::vec3& setRotation(glm::vec3 _rot) override;
	//glm::vec3 getScale() override;
	glm::vec3& setScale(glm::vec3 _scale) override;
	void processInputs(GLFWwindow* _window, InputManager _inputs);
	void processActions();

	AnimManager(Sprite* _sprite, std::string _characterData);
	AnimManager(Sprite* _sprite, std::map<Animation, int> _anims);
	AnimManager(std::string _characterData, glm::vec3 _position = glm::vec3(0, 0, 0));
	Palette getPalette();
	Palette mainPalette = Palette();
	Sprite* sprite;
	void setPalette(std::string _newColorFileName, std::string _oldColorFileName);
	void setPalette(int _index);

	std::map<int, Animation> animList;
	Frame* currentFrame;
	Animation* currentAnim;
	int frameCount = 0;
	std::vector<Palette> paletteList;
	int paletteIndex;
	std::string characterName;
	void setVelocity(glm::vec2 _vel, bool _additive);
	void setxVelocity(float _vel, bool _additive);
	void setyVelocity(float _vel, bool _additive);
	void setxAcceleration(float _accel, bool _additive);
	void setyAcceleration(float _accel, bool _additive);
protected:
	void parseXml(const char* _filename);
	void changeAnimation(int _index);
	void setAcceleration(glm::vec2 _acc);
	bool checkJumpLand();

	glm::vec2 velocity = glm::vec2(0, 0);
	glm::vec3 startPosition = glm::vec3(0, 0, 0);
	glm::vec2 acceleration = glm::vec2(0, 0);

	int loopIndex = 0;
};

