#include "Square.h"
#include <iostream>
#include "input.h"
#include "Core.h"
#include "GameGrid.h"

Square::Square(GameGrid* grid, unsigned int texture, int c, int r) : Button(texture)
{
	current_state = Empty;
	gameGrid = grid;
	column = c;
	row = r;
	input.AddButton(this);
	pieceID = 0;
}


void Square::Action()
{
	gameGrid->OnSquareClicked(column, row);
}



void Square::whoami()
{
	std::cout << "[col,row]: " << "<" << column << " , " << row << ">" << '\n';
	std::cout << "position : " << "<" << position.x << " , " << position.y << ">" << '\n';
	std::cout << "scale    : " << "<" << scale.x << " , " << scale.y << ">" << '\n' << '\n';
}


SquareState Square::GetState() { return current_state; }

void Square::SetState(SquareState newState, unsigned int pieceID) { current_state = newState; this->pieceID = pieceID; }

unsigned int Square::GetPieceID() { return pieceID; }

bool Square::IsOccupied() { return pieceID > 0; }


vec2 Square::GetCoordinate()
{
	return vec2(column,row);
}


void Square::LockPiece()
{
	isLocked = true;
}

void Square::UnlockPiece()
{
	isLocked = false;
}

bool Square::IsLocked()
{
	return isLocked;
}