#ifndef SQUARE_H
#define SQUARE_H
#include "Button.h"

class GameGrid;

enum SquareState { Empty, Player1, Player2 };

class Square : public Button
{
private:
	SquareState current_state;
	GameGrid* gameGrid;
	int column;
	int row;
	unsigned int pieceID;
	void whoami();
	bool isLocked;

protected:
	void Action();
public:
	Square(GameGrid* grid, unsigned int texture, int c, int r);
	SquareState GetState();
	void SetState(SquareState newState, unsigned int pieceID);
	vec2 GetCoordinate();
	unsigned int GetPieceID();
	bool IsOccupied();
	bool IsLocked();
	void LockPiece();
	void UnlockPiece();
};



#endif 


