#ifndef PIECE_H
#define PIECE_H
#include "GameObject.h"



class Piece : public GameObject
{
private:
	unsigned int pieceID;
public:
	Piece(unsigned int texture, unsigned int id);
	unsigned int GetPieceID();
};



#endif