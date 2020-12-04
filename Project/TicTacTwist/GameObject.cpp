#include "GameObject.h"


GameObject::GameObject(unsigned int texture)
{
	this->texture = texture;
	this->scale.x = 1;
	this->scale.y = 1;
	this->scale.z = 1;
	isOn = true;

}

vec3 GameObject::GetPosition()
{
	return position;
}

vec3 GameObject::GetRotation()
{
	return rotation;
}

vec3 GameObject::GetScale()
{
	return scale;
}




void GameObject::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void GameObject::SetPosition(vec3 position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}



void GameObject::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void GameObject::SetRotation(vec3 rotation)
{
	this->rotation.x = rotation.x;
	this->rotation.y = rotation.y;
	this->rotation.z = rotation.z;
}




void GameObject::SetScale(float x, float y, float z)
{
	scale.x = y;
	scale.y = x;
	scale.z = z;
}

void GameObject::SetScale(vec3 scale)
{
	SetScale(scale.x, scale.y, scale.z);
}


unsigned int GameObject::GetTexture()
{
	return texture;
}


void GameObject::SetRenderState(bool newState)
{
	isOn = newState;
}

bool GameObject::IsOn()
{
	return isOn;
}





