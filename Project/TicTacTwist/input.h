#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>              
#include <vector>
#include <iostream>
#include <vector>
#include "Button.h"



class Event
{
private:
	std::vector< class eListener* > listeners;
public:
	void attach(eListener* obs);
	void notify();
};


class eListener 
{
public:
	eListener(Event* current_event){current_event -> attach(this);}
	virtual void update() = 0;
};





class Test : public eListener
{
public:
	Test(Event* current_event) : eListener(current_event){}
	void update()
	{
		std::cout << "it workeD!\n";
	}
};




class Mouse
{
	private:
		double xpos;
		double ypos;
	public:
		Event onRightClick;
		void updatePosition(double x, double y) { xpos = x; ypos = y; };
		double getXPosition() { return xpos; };
		double getYPosition() { return ypos; };
};


class Input
{
private:
	Mouse mouse;
	std::vector<Button*> buttons;
public:
	void initialize(GLFWwindow* window)
	{		
	}
	Mouse& getMouse() { return mouse; };
	void AddButton(Button* button) { buttons.push_back(button); }
	void OnMouseLeftClick()
	{
		double mPosX = mouse.getXPosition();
		double mPosY = mouse.getYPosition();
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->SendClickCallback(mPosX, mPosY);
		}
		std::cout << "[mouse]: <" << mPosX / WINDOW_WIDTH << ", " << mPosY / WINDOW_HEIGHT << ">\n";
	}
};




#endif 
