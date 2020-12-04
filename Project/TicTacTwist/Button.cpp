#include "Button.h"
#include <iostream>




Button::Button(unsigned int texture) : GameObject(texture)
{
	isInteractable = true;
}



void Button::OnClick()
{
	for (int i = 0; i < listeners.size(); i++) listeners[i]();
}

void Button::Action()
{
}


void Button::AddOnClickListener(void (*func)())
{
	listeners.push_back(func);
}


void Button::SetInteractable(bool value)
{
	isInteractable = value;
}



void Button::SendClickCallback(double mousePosX, double mousePosY)
{
	double click_x = mousePosX / WINDOW_WIDTH;
	double click_y = mousePosY / WINDOW_HEIGHT;

	double objectPosX = (position.x + 1) / 2;
	double objectPosY = -(position.y - 1) / 2;

	double topLeftX = objectPosX - (scale.x / 2);
	double topLeftY = objectPosY - (scale.y / 2);

	double bottomRightX = objectPosX + (scale.x / 2);
	double bottomRightY = objectPosY + (scale.y / 2);

	// Check if mouse clicked buttons:
	if (topLeftX < click_x && topLeftY < click_y && click_x < bottomRightX && click_y < bottomRightY)
	{
		if(isInteractable) Action();
		OnClick();
	}
}

