#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "VecMat.h"

class GameObject
{
protected:
	bool isOn;
	unsigned int texture;
	vec3 position;
	vec3 rotation;
	vec3 scale;
public:
	GameObject(unsigned int texture);
	vec3 GetPosition();
	vec3 GetRotation();
	vec3 GetScale();
	void SetPosition(float x, float y, float z);
	void SetPosition(vec3 position);
	void SetRotation(float x, float y, float z);
	void SetRotation(vec3 rotation);
	void SetScale(float x, float y, float z);
	void SetScale(vec3 scale);
	unsigned int GetTexture();
	void SetRenderState(bool);
	bool IsOn();
};


#endif 
