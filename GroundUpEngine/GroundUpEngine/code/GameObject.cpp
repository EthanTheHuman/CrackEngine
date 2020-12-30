#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::init()
{
}

void GameObject::update()
{
}

void GameObject::render()
{
}

glm::vec3& GameObject::getPosition()
{
	return position;
}

glm::vec3& GameObject::setPosition(glm::vec3 _pos)
{
	position = _pos;      
	return position;
}

glm::vec3& GameObject::getRotation()
{
	return rotation;
}

glm::vec3& GameObject::setRotation(glm::vec3 _rot)
{
	rotation = _rot;
	return rotation;
}

glm::vec3& GameObject::getScale()
{
	return scale;
}

glm::vec3& GameObject::setScale(glm::vec3 _scale)
{
	scale = _scale;
	return scale;
}
