#include "input.h"



void Event::attach(eListener* obs)
{
	listeners.push_back(obs);
}

void Event::notify()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]->update();
	}
}




