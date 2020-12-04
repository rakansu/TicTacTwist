#include "GameGrid.h"
#include <iostream>
#include "Gameplay.h"



GameGrid::GameGrid(unsigned int texture, unsigned int square_texture) : GameObject(texture)
{
	this->square_texture = square_texture;
}



void GameGrid::Initialize()
{
	// Creating Grid:
	for (int c = 0; c < COLUMNS; c++)
	{
		std::vector<Square*> row;
		for (int r = 0; r < ROWS; r++)
		{
			Square* square = new Square(this, square_texture, c, r);
			row.push_back(square);
			render_buffer.push_back(square);
		}
		grid.push_back(row);
	}

	// Remap the squares on the grid:
	for (int c = 0; c < COLUMNS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			double x_scale = (scale.x / COLUMNS);
			double y_scale = (scale.y / ROWS);
			double x_pos = position.x + 2 * ((-(scale.x / 2.0)) + c * x_scale) + x_scale;
			double y_pos = position.y + 2 * (((scale.y / 2.0)) - r * y_scale) - y_scale;
			grid[c][r]->SetPosition(x_pos, y_pos, 0);
			grid[c][r]->SetScale(x_scale - 0.01, y_scale - 0.01, 0);
		}
	}
	SetScale(scale.x + 0.019, scale.y + 0.019, 0);
}


void GameGrid::AssignGameplay(Gameplay* gameplay)
{
	this->gameplay = gameplay;
}




void GameGrid::OnSquareClicked(int c, int r)
{
	gameplay->OnSquareClicked(c, r);
}

Square* GameGrid::GetSquare(int c, int r)
{
	return grid[c][r];
}

std::vector< std::vector<Square*> > GameGrid::GetMatrix()
{
	return grid;
}




