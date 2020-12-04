#ifndef CARD_H
#define CARD_H

#include "Button.h"
#include "Gameplay.h"

class Card : public Button
{
private:
	Gameplay* gameplay;
	bool isPlayer1;
	GameObject* card;
	int id;
protected:
	void Action();

public:
	Card(Gameplay* gameplay,unsigned int texture, int id, bool isplayer1);
	~Card();
	void SetCardPosition(float, float, float);
	bool IsPlayer1();
	int GetID();
	void HideCard();
};



#endif