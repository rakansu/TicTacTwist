#ifndef BUTTON_H
#define BUTTON_H
#include "Core.h"
#include <vector>
#include "GameObject.h"


class Button : public GameObject
{
private:
	bool isInteractable;
	std::vector< void (*)() > listeners;
	void OnClick();
protected:
	virtual void Action();
public:
	Button(unsigned int texture);
	void AddOnClickListener(void (*func)());
	void SendClickCallback(double x, double y);
	void SetInteractable(bool);
};








#endif
