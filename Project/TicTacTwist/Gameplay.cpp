#include "Gameplay.h"
#include "GameGrid.h"
#include "Square.h"
#include "Core.h"




Gameplay::Gameplay(GameGrid* grid)
{
	gamegrid = grid;
	isPlayer1 = true;
	current_round = 1;
	current_pieceID = 1;
	player1_piecesCount = 5;
	player2_piecesCount = 5;
	player1_turnsLocked = 0;
	player2_turnsLocked = 0;

	for (int i = 0; i < 6; i++)
	{
		GameObject* text1 = new GameObject(text_textures[i]);
		GameObject* text2 = new GameObject(text_textures[i]);
		// Set Scale:
		text1->SetScale(0.07, 0.07, 0);
		text2->SetScale(0.07, 0.07, 0);

		// Set Position:
		text1->SetPosition(-0.55, 0.85, 0);
		text2->SetPosition(0.85, 0.85, 0);

		// Push to Render Buffer:
		render_buffer.push_back(text1);
		render_buffer.push_back(text2);

		// Keep a reference:
		player1_texts.push_back(text1);
		player2_texts.push_back(text2);

		text1->SetRenderState(false);
		text2->SetRenderState(false);
	}
	player1_texts[5]->SetRenderState(true);
	player2_texts[5]->SetRenderState(true);

	// Cards:
	for (int i = 1; i < 4; i++)
	{
		Card* p1_card = new Card(this,card_textures[i], i, true);
		Card* p2_card = new Card(this,card_textures[i], i, false);
		double x_ratio = (i - 1) * 0.25;
		p1_card->SetCardPosition(-0.8 + x_ratio, -0.8, 0);
		p2_card->SetCardPosition(0.3 + x_ratio, -0.8, 0);

		render_buffer.push_back(p1_card);
		render_buffer.push_back(p2_card);
	}

	selected_piece_effect = new GameObject(misc_textures[0]);
	player1_locked_effect = new GameObject(misc_textures[1]);
	player2_locked_effect = new GameObject(misc_textures[1]);
	card_selected_effect = new GameObject(misc_textures[2]);

	dim_screen = new GameObject(misc_textures[3]);
	winnerbar = new GameObject(misc_textures[4]);;
	p1_win = new GameObject(misc_textures[5]);;
	p2_win = new GameObject(misc_textures[6]);;

	selected_piece_effect->SetRenderState(false);
	player1_locked_effect->SetRenderState(false);
	player2_locked_effect->SetRenderState(false);
	card_selected_effect->SetRenderState(false);

	dim_screen->SetRenderState(false);
	winnerbar->SetRenderState(false);
	p1_win->SetRenderState(false);
	p2_win->SetRenderState(false);

	render_buffer.push_back(selected_piece_effect);
	render_buffer.push_back(player1_locked_effect);
	render_buffer.push_back(player2_locked_effect);
	render_buffer.push_back(card_selected_effect);

	render_buffer2.push_back(dim_screen);
	render_buffer2.push_back(p1_win);
	render_buffer2.push_back(p2_win);
	render_buffer2.push_back(winnerbar);

}


Gameplay::~Gameplay()
{
	for (int i = 0; i < 6; i++)
	{
		delete player1_texts[i];
		delete player2_texts[i];
	}
	delete selected_piece_effect;
	delete player1_locked_effect;
	delete player2_locked_effect;
	delete dim_screen;
	delete winnerbar;
	delete p1_win;
	delete p2_win;
}


void Gameplay::SwitchTurns()
{
	isPlayer1 = !isPlayer1;
	player1_turn->SetRenderState(false);
	player2_turn->SetRenderState(false);
	if (isPlayer1) player1_turn->SetRenderState(true);
	else player2_turn->SetRenderState(true);
	current_round++;
	DeactivateSM();

	if (player1_turnsLocked > 0) player1_turnsLocked--;
	else
	{
		if(player1_lockedSquare != nullptr) player1_lockedSquare->UnlockPiece();
		player1_locked_effect->SetRenderState(false);
	}

	if (player2_turnsLocked > 0) player2_turnsLocked--;
	else
	{
		if (player2_lockedSquare != nullptr) player2_lockedSquare->UnlockPiece();
		player2_locked_effect->SetRenderState(false);
	}
}



void Gameplay::OnSquareClicked(int col, int row)
{
	std::cout << "[Round " << current_round << "]";
	std::cout << "  --" << ((isPlayer1) ? "Player 1" : "Player 2") << " Turn\n";

	Square* square = gamegrid->GetSquare(col, row);
	SquareState state = (isPlayer1) ? Player1 : Player2;
	
	
	if (square->IsLocked()) return;

	// 1- Has to be a neighbor square 
	// 2- has to be an empty square 
	// 3- it has to be a legal coordinate 
	if (IsSquareSelected())
	{

		if (current_sm == Reposition)
		{
			if (square->GetState() == Empty)
			{
				// Remove Special Move from the player who used it
				selected_card->HideCard();
				// Reposition:
				MovePiece(square, state);
				// Check win:
				bool hasWon = DidPlayerWin(vec2(col, row), isPlayer1);
				if (hasWon) ShowWinScreen(isPlayer1);
				// End Turn:
				SwitchTurns();
				// Unselect piece:
				selected_square = nullptr;
				selected_piece_effect->SetRenderState(false);
				card_selected_effect->SetRenderState(false);
			}
			else
			{
				// Invalid Move:

			}
			return;
		}

		if (current_sm == Switch)
		{
			if (selected_square -> GetState() != square-> GetState() && square -> GetState() != Empty && selected_square -> GetState() != Empty)
			{
				// Remove Special Move from the player who used it:
				selected_card->HideCard();
				// Switch Pieces:
				SwitchPieces(selected_square, square);
				selected_square = nullptr;
				// Check win:
				bool hasWon = DidPlayerWin(vec2(col, row), isPlayer1);
				if (hasWon) ShowWinScreen(isPlayer1);

				// End Turn:
				SwitchTurns();
				// Unselect piece:
				selected_square = nullptr;
				selected_piece_effect->SetRenderState(false);
				card_selected_effect->SetRenderState(false);
			}
			else
			{
				// Invalid Move:

			}
			return;
		}



		if (square->GetState() != Empty)
		{
			selected_square = nullptr;
			// Unselect piece:
			selected_piece_effect->SetRenderState(false);
			return;
		}


		int x = square->GetCoordinate().x - selected_square->GetCoordinate().x;
		int y = square->GetCoordinate().y - selected_square->GetCoordinate().y;

		// Clamp between 0 and 1 [im avoiding negative because I dont like it]
		x = (x < 0) ? -1 * x : x;
		y = (y < 0) ? -1 * y : y;
		
		// Neighboring Square:
		if (x == 0 && y == 0) selected_square = nullptr;
		if (0 <= x && x <= 1 && 0 <= y && y <= 1)
		{
			MovePiece(square, state);
			// Check win
			bool hasWon = DidPlayerWin(vec2(col, row), isPlayer1);
			if (hasWon) ShowWinScreen(isPlayer1);

			SwitchTurns();
			// Unselect piece:
			selected_square = nullptr;
			selected_piece_effect->SetRenderState(false);
			card_selected_effect->SetRenderState(false);
		}
		return;
	}

	switch (square -> GetState())
	{
	case Empty:
		if (isPlayer1 && player1_piecesCount > 0)
		{
			player1_texts[player1_piecesCount]->SetRenderState(false);
			square->SetState(state, current_pieceID);
			CreatePiece(square);
			player1_texts[player1_piecesCount]->SetRenderState(true);
			// Check win
			bool hasWon = DidPlayerWin(vec2(col, row), true);
			if (hasWon) ShowWinScreen(isPlayer1);

			SwitchTurns();
		}
		else if (!isPlayer1 && player2_piecesCount > 0)
		{
			player2_texts[player2_piecesCount]->SetRenderState(false);
			square->SetState(state, current_pieceID);
			CreatePiece(square);
			player2_texts[player2_piecesCount]->SetRenderState(true);
			// Check win
			bool hasWon = DidPlayerWin(vec2(col, row), false);
			if (hasWon) ShowWinScreen(isPlayer1);

			SwitchTurns();
		}
		break;
	case Player1:
		if (isPlayer1)
		{
			selected_square = square;
			selected_piece_effect->SetRenderState(true);
			selected_piece_effect->SetScale(square->GetScale());
			selected_piece_effect->SetPosition(square->GetPosition());
		}
		else
		{
			if (current_sm == Lock)
			{
				// Remove Special Move from the player who used it:
				selected_card->HideCard();
				// Lock Piece:
				square->LockPiece();
				player1_lockedSquare = square;
				player1_turnsLocked = 1;
				player1_locked_effect->SetScale(square->GetScale());
				player1_locked_effect->SetPosition(square->GetPosition());
				player1_locked_effect->SetRenderState(true);
				card_selected_effect->SetRenderState(false);

			}
		}
		return;
	case Player2:
		if (!isPlayer1)
		{
			selected_square = square;
			selected_piece_effect->SetRenderState(true);
			selected_piece_effect->SetScale(square->GetScale());
			selected_piece_effect->SetPosition(square->GetPosition());
		}
		else
		{
			if (current_sm == Lock)
			{
				// Remove Special Move from the player who used it:
				selected_card->HideCard();
				// Lock Piece:
				square->LockPiece();
				player2_lockedSquare = square;
				player2_turnsLocked = 1;
				player2_locked_effect->SetScale(square->GetScale());
				player2_locked_effect->SetPosition(square->GetPosition());
				player2_locked_effect->SetRenderState(true);
				card_selected_effect->SetRenderState(false);
			}
		}
		return;
	}
}



void Gameplay::CreatePiece(Square* square)
{
	unsigned int piece_texture = (isPlayer1) ? PLAYER1_PIECE_TEXTURE : PLAYER2_PIECE_TEXTURE;
	
	Piece* piece = new Piece(piece_texture, current_pieceID);
	
	piece->SetScale(square->GetScale());
	piece->SetPosition(square->GetPosition());

	if (isPlayer1)
	{
		player1_pieces.push_back(piece);
		player1_piecesCount--;
	}
	else
	{
		player2_pieces.push_back(piece);
		player2_piecesCount--;
	}

	render_buffer.push_back(piece);
	current_pieceID++;	
	Audio->play2D("../../Resources/audio/place_piece.mp3", false);
}

void Gameplay::MovePiece(Square* next_square, SquareState state)
{
	if (selected_square == nullptr) return;
	unsigned int pieceID = selected_square->GetPieceID();
	std::vector<Piece*> pieces = (isPlayer1) ? player1_pieces : player2_pieces;
	Piece* piece = GetPiece(pieceID, pieces);

	selected_square->SetState(Empty, 0);
	next_square->SetState(state, pieceID);
	piece->SetPosition(next_square->GetPosition());
	selected_square = nullptr;

	Audio->play2D("../../Resources/audio/piece_move.mp3", false);
}



void Gameplay::SwitchPieces(Square* A, Square* B)
{	
	if (A == nullptr || B == nullptr) return;
	std::vector<Piece*> A_pieces = (A->GetState() == Player1) ? player1_pieces : player2_pieces;
	std::vector<Piece*> B_pieces = (B->GetState() == Player1) ? player1_pieces : player2_pieces;

	Piece* A_piece = GetPiece(A->GetPieceID(), A_pieces);
	Piece* B_piece = GetPiece(B->GetPieceID(), B_pieces);

	A_piece->SetPosition(B->GetPosition());
	B_piece->SetPosition(A->GetPosition());

	SquareState tmp_state = A->GetState();
	unsigned int tmp_pieceID = A->GetPieceID();

	A->SetState(B->GetState(), B->GetPieceID());
	B->SetState(tmp_state, tmp_pieceID);
	Audio->play2D("../../Resources/audio/piece_move.mp3", false);
}




Piece* Gameplay::GetPiece(unsigned int pieceID, std::vector<Piece*> pieces)
{
	for (int i = 0; i < pieces.size(); i++)
		if (pieces[i]->GetPieceID() == pieceID)
			return pieces[i];
}




bool Gameplay::IsSquareSelected() { return selected_square != nullptr;}



void Gameplay::AssignPlayer1TurnObject(GameObject* turnObj)
{
	player1_turn = turnObj;
}

void Gameplay::AssignPlayer2TurnObject(GameObject* turnObj)
{
	player2_turn = turnObj;
}


bool Gameplay::DidPlayerWin(vec2 coord, bool isPlayer1)
{
	SquareState state = (isPlayer1) ? Player1 : Player2;

	int horizontal_count = CountSquares(coord, vec2(-1, 0), state) + CountSquares(coord, vec2(1, 0), state) + 1;
	int vertical_count = CountSquares(coord, vec2(0, -1), state) + CountSquares(coord, vec2(0, 1), state) + 1;
	int first_diagonal_count = CountSquares(coord, vec2(-1, -1), state) + CountSquares(coord, vec2(1, 1), state) + 1;
	int second_diagonal_count = CountSquares(coord, vec2(1, -1), state) + CountSquares(coord, vec2(-1, 1), state) + 1;

	//std::cout << "horizontal count: " << horizontal_count << '\n';
	//std::cout << "vertical count: " << vertical_count << '\n';
	//std::cout << "diag1 count: " << first_diagonal_count << '\n';
	//std::cout << "diag2 count: " << second_diagonal_count << '\n';


	return (horizontal_count == WIN_CONDITION || vertical_count == WIN_CONDITION || first_diagonal_count == WIN_CONDITION || second_diagonal_count == WIN_CONDITION);
}


int Gameplay::CountSquares(vec2 coord, vec2 direction , SquareState state)
{
	std::vector< std::vector<Square*> > grid = gamegrid->GetMatrix();
	int count = 0;
	int x = 0 + direction.x;
	int y = 0 + direction.y;

	while (0 <= (coord.x + x) && (coord.x + x) < 5 && 0 <= (coord.y + y) && (coord.y + y) < 5)
	{
		int c = coord.x + x;
		int r = coord.y + y;
		if (grid[c][r]->GetState() == state) count++;
		x += direction.x;
		y += direction.y;
	}
	return count;
}



void Gameplay::ActivateSM(SpecialMove sm)
{
	current_sm = sm;
}

void Gameplay::DeactivateSM()
{
	current_sm = None;
	card_selected_effect->SetRenderState(false);
}




void Gameplay::OnCardClick(Card* card)
{
	// Card Processing:
	if (isPlayer1 != card->IsPlayer1()) return;
	current_sm = SpecialMove(card->GetID());
	selected_card = card;
	card_selected_effect->SetPosition(card->GetPosition());
	card_selected_effect->SetScale(card->GetScale());
	card_selected_effect->SetRenderState(true);
}



void Gameplay::ShowWinScreen(bool isPlayer1)
{
	dim_screen->SetRenderState(true);
	winnerbar->SetRenderState(true);
	if (isPlayer1) p1_win->SetRenderState(true);
	else p2_win->SetRenderState(true);

	Audio->play2D("../../Resources/audio/Win.mp3", false);
}