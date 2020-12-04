#ifndef GAMEGRID_H
#define GAMEGRID_H
#include "GameObject.h"
#include "Button.h"
#include <vector>
#include "Square.h"

class Gameplay;

class GameGrid : public GameObject
{
private: 
	Gameplay* gameplay;
	const int COLUMNS = 5;
	const int ROWS = 5;
	std::vector< std::vector<Square*> > grid;
	unsigned int square_texture;
public:
	GameGrid(unsigned int texture, unsigned int square_texure);
	void AssignGameplay(Gameplay* gameplay);
	void Initialize();
	void OnSquareClicked(int c, int r);
	std::vector< std::vector<Square*> > GetMatrix();
	Square* GetSquare(int c, int r);
};


#endif // !GAMEGRID_H
