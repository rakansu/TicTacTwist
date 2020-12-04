#include "Piece.h"



Piece::Piece(unsigned int texture, unsigned int id) : GameObject(texture)
{
	pieceID = id;
}

unsigned int Piece::GetPieceID() { return pieceID; }



