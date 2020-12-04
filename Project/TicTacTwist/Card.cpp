#include "Card.h"
#include "input.h"


Card::Card(Gameplay* gameplay,unsigned int texture, int id, bool isPlayer1) : Button(texture)
{
	this->gameplay = gameplay;
	this -> SetScale(0.5 * 0.2 , 1 * 0.2, 0);
	SetRenderState(false);
	card = new GameObject(texture);
	card->SetScale(0.2, 0.2, 1);
	render_buffer.push_back(card);
	this->id = id;
	this->isPlayer1 = isPlayer1;
	input.AddButton(this);
}

void Card::Action()
{
	gameplay->OnCardClick(this);
}


void Card::SetCardPosition(float x, float y, float z)
{
	SetPosition(x, y, z);
	card->SetPosition(x, y, z);
}




Card::~Card()
{
	delete card;
}


bool Card::IsPlayer1() { return isPlayer1; }

int Card::GetID() { return id; }


void Card::HideCard()
{
	card->SetRenderState(false);
	SetInteractable(false);
}