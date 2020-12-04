#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "GameGrid.h"
#include <vector>
#include "Piece.h"
#include "Card.h"

enum SpecialMove { None, Reposition, Switch, Lock };

class Card;


class Gameplay
{
private:
	SpecialMove current_sm;
	bool isPlayer1;
	unsigned int WIN_CONDITION = 4;
	unsigned int player1_piecesCount;
	unsigned int player2_piecesCount;
	int current_round;
	unsigned int current_pieceID;
	GameGrid* gamegrid;
	GameObject* player1_turn;
	GameObject* player2_turn;
	Square* selected_square;
	std::vector<Piece* > player1_pieces;
	std::vector<Piece* > player2_pieces;
	std::vector<GameObject*> player1_texts;
	std::vector<GameObject*> player2_texts;
	Card* selected_card;
	std::vector<Card*> player1_cards;
	std::vector<Card*> player2_cards;
	Square* player1_lockedSquare;
	Square* player2_lockedSquare;
	int player1_turnsLocked;
	int player2_turnsLocked;
	GameObject* selected_piece_effect;
	GameObject* player1_locked_effect;
	GameObject* player2_locked_effect;
	GameObject* card_selected_effect;
	GameObject* dim_screen;
	GameObject* winnerbar;
	GameObject* p1_win;
	GameObject* p2_win;
	void SwitchTurns();
	void CreatePiece(Square* square);
	Piece* GetPiece(unsigned int pieceID, std::vector<Piece*>);
	bool IsSquareSelected();
	bool DidPlayerWin(vec2 coord, bool isPlayer1);
	int CountSquares(vec2 coord, vec2 direction, SquareState state);
	void MovePiece(Square* nextSquare, SquareState);
	void SwitchPieces(Square*, Square*);
	void ShowWinScreen(bool isPlayer1);
public:
	Gameplay(GameGrid* grid);
	~Gameplay();
	void OnSquareClicked(int c, int r);
	void AssignPlayer1TurnObject(GameObject*);
	void AssignPlayer2TurnObject(GameObject*);
	void ActivateSM(SpecialMove);
	void DeactivateSM();
	void OnCardClick(Card* card);
};




             


#endif 
