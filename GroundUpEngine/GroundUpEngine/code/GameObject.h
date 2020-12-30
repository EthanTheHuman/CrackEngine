#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void init();
	virtual void update();
	virtual void render();

	// Transforms
	glm::vec3& getPosition();
	glm::vec3& setPosition(glm::vec3 _pos);
	glm::vec3& getRotation();
	glm::vec3& setRotation(glm::vec3 _rot);
	glm::vec3& getScale();
	glm::vec3& setScale(glm::vec3 _scale);

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

