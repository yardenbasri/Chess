#include "Game.h"

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_SIZE, SCREEN_SIZE), "Yarden`s Chess");	
	checkmate = 0;
	stalemate = 0;
	gameOver = 0;
	whiteTurn = 1;
	flag = 1;
}

void Game::Start()
{
	LoadData();
	CreateBoard();
	LoadSprites();
	Init();
}

void Game::LoadData()
{
	boardTex.loadFromFile(BOARD_IMG);
	std::string pieces[] = { "bR", "bN", "bB", "bK", "bQ","bp","wR", "wN", "wB", "wK", "wQ","wp" };
	for (auto piece : pieces)
	{
		piecesTex[piece].loadFromFile("Assets/" + piece + ".png");
	}
	
	font.loadFromFile("Assets/arial.ttf");
	text_board.setFont(font);
	text_board.setCharacterSize(16);
	text_board.setFillColor(sf::Color::Black);
	text_board.setOutlineThickness(1);
	text_board.setOutlineColor(sf::Color::White);
	
	text_msg.setFont(font);
	text_msg.setCharacterSize(60);
	text_msg.setFillColor(sf::Color::White);
	text_msg.setPosition(100, 350);
	
}

void Game::LoadSprites()
{
	board.setTexture(boardTex);
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			std::string p = boardStatus[i][j];
			switch (p[1])
			{
			case 'p':
				pieces[i][j] = new Pown(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'K':
				pieces[i][j] = new King(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				
				break;
			case 'Q':
				pieces[i][j] = new Queen(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'R':
				pieces[i][j] = new Rock(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i,j), p);
				break;
			case 'B':
				pieces[i][j] = new Bishop(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'N':
				pieces[i][j] = new Knight(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;

			default:
				pieces[i][j] = NULL;
				break;
			}
		}
	}
	
	
}

void Game::CreateBoard()
{
	sf::RectangleShape rectangle(sf::Vector2f(100, 100));
	//rectangle.setSize(sf::Vector2f(115, 115));
	
	sf::Event event;
	int mod = 0;
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			rectangle.setPosition(100 * j, 100 * i);
			if (j % 2 == mod)
			{
				rectangle.setFillColor(sf::Color(165, 42, 42));//(160,82,45);
				rectangle.setOutlineThickness(1);
				rectangle.setOutlineColor(sf::Color::Black);
			}
			else
			{
				rectangle.setFillColor(sf::Color(241, 217, 192));
				rectangle.setOutlineThickness(1);
				rectangle.setOutlineColor(sf::Color::Black);
			}
			squres.push_back(rectangle);

		}
		mod = !mod;
	}
}

void Game::DrawNumbersLetters(sf::RenderWindow* window)
{
	sf::Vector2f pos = { 5 ,775 };
	std::string letters[] = { "a", "b", "c", "d", "e", "f", "g", "h"};
	text_board.setPosition(pos);
	for (auto letter : letters)
	{
		text_board.setString(letter);
		window->draw(text_board);
		pos.x += DELTA_POS;
		text_board.setPosition(pos);
	}
	pos = { 785 ,700 };
	text_board.setPosition(pos);
	std::string numbers[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
	for (auto number : numbers)
	{
		text_board.setString(number);
		window->draw(text_board);
		pos.y -= 100;
		text_board.setPosition(pos);
	}
	
	
	
}



sf::Vector2f Game::Indexes2Position(sf::Vector2i indexes)
{
	return sf::Vector2f(indexes.y * DELTA_POS + START_POS, indexes.x * DELTA_POS + START_POS);
}

sf::Vector2i Game::Position2Indexes(sf::Vector2f mPos)
{
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (squres[i*DIMENSIONS+j].getGlobalBounds().contains(mPos))
			{
				return sf::Vector2i(i, j);
			}
		}
	}
}

void Game::Move()
{
	sf::Vector2i indexesBefore = playerClickes[0];
	sf::Vector2i indexesAfter = playerClickes[1];
	if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "bp" && indexesAfter.x == 7)  // check for bp promotion.
	{
		pieces[indexesAfter.x][indexesAfter.y] = new Queen(piecesTex["bQ"], Indexes2Position(indexesAfter), 0, indexesAfter, "bQ");
		pieces[indexesBefore.x][indexesBefore.y] = NULL;

		
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "wp" && indexesAfter.x == 0)  // check for wp promotion.
	{
		pieces[indexesAfter.x][indexesAfter.y] = new Queen(piecesTex["wQ"], Indexes2Position(indexesAfter), 1, indexesAfter, "wQ");
		pieces[indexesBefore.x][indexesBefore.y] = NULL;
		
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "wp" &&
		indexesBefore.x == 3 && indexesAfter.y != indexesBefore.y && !pieces[indexesAfter.x][indexesAfter.y]) // check white en passent
	{
		pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(playerClickes[1]);
		pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(playerClickes[1]));
		pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
		pieces[indexesBefore.x][indexesBefore.y] = NULL;
		pieces[indexesAfter.x + 1][indexesAfter.y] = NULL; // delete the pown eaten.	
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "bp" && indexesBefore.x == 4 &&
		indexesAfter.y != indexesBefore.y && !pieces[indexesAfter.x][indexesAfter.y])  // check black en passent
	{
		pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(playerClickes[1]);
		pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(playerClickes[1]));
		pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
		pieces[indexesBefore.x][indexesBefore.y] = NULL;
		pieces[indexesAfter.x - 1][indexesAfter.y] = NULL; // delete the pown eaten.

	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType()[1] == 'K' && abs(indexesAfter.y-indexesBefore.y) == 2 ) // check castling
	{
		pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(playerClickes[1]);
		pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(playerClickes[1]));
		pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
		pieces[indexesBefore.x][indexesBefore.y] = NULL;
		if (indexesAfter.y > indexesBefore.y)// && !squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y-1)) && !squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y - 2))) // king castling
		{
			pieces[indexesAfter.x][indexesAfter.y + 1]->SetIndexes(sf::Vector2i(indexesAfter.x, indexesAfter.y - 1));
			pieces[indexesAfter.x][indexesAfter.y + 1]->SetPosition(Indexes2Position(sf::Vector2i(indexesAfter.x, indexesAfter.y - 1)));
			pieces[indexesAfter.x][indexesAfter.y - 1] = pieces[indexesAfter.x][indexesAfter.y + 1];
			pieces[indexesAfter.x][indexesAfter.y + 1] = NULL;
		}
		else //if(!squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1)) && !squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y + 2))) //queen catling
		{
			pieces[indexesAfter.x][indexesAfter.y - 2]->SetIndexes(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1));
			pieces[indexesAfter.x][indexesAfter.y - 2]->SetPosition(Indexes2Position(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1)));
			pieces[indexesAfter.x][indexesAfter.y + 1] = pieces[indexesAfter.x][indexesAfter.y - 2];
			pieces[indexesAfter.x][indexesAfter.y - 2] = NULL;
		}

	}
	else
	{
		pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(playerClickes[1]);
		pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(playerClickes[1]));
		pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
		pieces[indexesBefore.x][indexesBefore.y] = NULL;
	}
	
	
}

void Game::MarkSqure(sf::Vector2i indexes)
{
	if (squres[indexes.x * DIMENSIONS + indexes.y].getFillColor() == sf::Color(165, 42, 42))
	{
		squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(sf::Color(255, 255, 201));
	}
	if (squres[indexes.x * DIMENSIONS + indexes.y].getFillColor() == sf::Color(241, 217, 192))
	{
		squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(sf::Color(255, 255, 200));
	}
}

void Game::cleanSqure(sf::Vector2i indexes)
{
	if (squres[indexes.x * DIMENSIONS + indexes.y].getFillColor() == sf::Color(255,255,201))
	{
		squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(sf::Color(165, 42, 42));
	}
	else if (squres[indexes.x * DIMENSIONS + indexes.y].getFillColor() == sf::Color(255, 255, 200))
	{
		squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(sf::Color(241, 217, 192));
	}
}






double Game::calculateDistance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt(pow(a.x - b.x, 2) +
		pow(a.y - b.y, 2));
}

void Game::CleanBoard()
{
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			free(pieces[i][j]);
			std::string p = boardStatus[i][j];
			switch (p[1])
			{
			case 'p':
				pieces[i][j] = new Pown(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i,j), p);
				break;
			case 'K':
				pieces[i][j] = new King(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'Q':
				pieces[i][j] = new Queen(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'R':
				pieces[i][j] = new Rock(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i,j), p);
				break;
			case 'B':
				pieces[i][j] = new Bishop(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'N':
				pieces[i][j] = new Knight(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;

			default:
				pieces[i][j] = NULL;
				break;
			}
		}
	}
	whiteTurn = 1;
	checkmate = 0;
	stalemate = 0;
	gameOver = 0;
		
	
}

std::vector<sf::CircleShape> Game::GetPossibleCircles(std::vector<sf::Vector2i> possibleMoves)
{
	std::vector<sf::CircleShape> circles;
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Black);
	circle.setRadius(15);
	for (auto move : possibleMoves) {
		sf::Vector2f pos = Indexes2Position(move);
		pos.x += 13;
		pos.y += 10;
		circle.setPosition(pos);
		circles.push_back(circle);
	}
	return circles;
}

void Game::Castling(sf::Vector2i king, sf::Vector2i rock, int rightCastling)
{
	if (rightCastling)
	{
		pieces[king.x][king.y]->SetIndexes(sf::Vector2i(king.x, king.y + 2));
		pieces[king.x][king.y]->SetPosition(Indexes2Position(sf::Vector2i(king.x, king.y + 2)));
		pieces[rock.x][rock.y]->SetIndexes(sf::Vector2i(rock.x, rock.y - 2));
		pieces[rock.x][rock.y]->SetPosition(Indexes2Position(sf::Vector2i(rock.x, rock.y - 2)));
		pieces[king.x][king.y + 2] = pieces[king.x][king.y];
		pieces[rock.x][rock.y - 2] = pieces[rock.x][rock.y];
		pieces[king.x][king.y] = NULL;
		pieces[rock.x][rock.y] = NULL;
	}
	else
	{
		pieces[king.x][king.y]->SetIndexes(sf::Vector2i(king.x, king.y - 2));
		pieces[king.x][king.y]->SetPosition(Indexes2Position(sf::Vector2i(king.x, king.y - 2)));
		pieces[rock.x][rock.y]->SetIndexes(sf::Vector2i(rock.x, rock.y + 3));
		pieces[rock.x][rock.y]->SetPosition(Indexes2Position(sf::Vector2i(rock.x, rock.y + 3)));
		pieces[king.x][king.y - 2] = pieces[king.x][king.y];
		pieces[rock.x][rock.y + 3] = pieces[rock.x][rock.y];
		pieces[king.x][king.y] = NULL;
		pieces[rock.x][rock.y] = NULL;
	}
}

int Game::IsCastlingLegal()
{
	sf::Vector2i indexesBefore = playerClickes[0];
	sf::Vector2i indexesAfter = playerClickes[1];
	if (pieces[indexesBefore.x][indexesBefore.y]->GetType()[1] == 'K' && abs(indexesAfter.y - indexesBefore.y) == 2) // check if the move was castling.
	{
		if (InCheck())
			return 0;
		if (indexesAfter.y > indexesBefore.y )  // king castling
		{
			if(!squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y - 1)))
				return 1;
		}
		else if(!squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1))) //queen catling
		{
			return 1;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}

std::vector<sf::Vector2i> Game::GetLegalMoves(Piece* piece)
{
	std::vector<sf::Vector2i> moves = piece->GetPossibleMoves(pieces);
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		// Do the move.
		sf::Vector2i indexesBefore = piece->GetIndexes();
		sf::Vector2i indexesAfter = moves[i];
		pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(indexesAfter);
		pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(indexesAfter));
		auto temp_piece = pieces[indexesAfter.x][indexesAfter.y];
		pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
		pieces[indexesBefore.x][indexesBefore.y] = NULL;
		if (InCheck())
			moves.erase(moves.begin()+i);
		// Undo the move.
		auto temp_indexes = indexesBefore;
		indexesBefore = indexesAfter;
		indexesAfter = temp_indexes;
		pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(indexesAfter);
		pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(indexesAfter));
		pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
		pieces[indexesBefore.x][indexesBefore.y] = temp_piece;

	}
	return moves;
}

int Game::InCheck()
{
	std::vector<sf::Vector2i> locations = GetKingsLocation();
	if (whiteTurn)
		return squreUnderAttack(locations[0]); // White king`s indexes.
	else
		return squreUnderAttack(locations[1]); // Black king`s indexes.
}

int Game::squreUnderAttack(sf::Vector2i location)
{
	whiteTurn = !whiteTurn; // switch turns to check if the opponent can a attack the squre.
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j] && pieces[i][j]->IsWhite() == whiteTurn) // check if its white`s turn and the piece is white or if it` s black turn and the piece is black.
			{
				auto opponentPossibleMoves = pieces[i][j]->GetPossibleMoves(pieces);
				for (auto move : opponentPossibleMoves)
				{
					if (move == location) {
						whiteTurn = !whiteTurn; // switch turns back.
						return 1;
					}
				}
			}
		}
	}
	whiteTurn = !whiteTurn; // switch turns back.
	return 0;
}

std::vector<sf::Vector2i> Game::GetKingsLocation()
{
	std::vector<sf::Vector2i> locations;
	locations.resize(2);
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j] && pieces[i][j]->GetType() == "wK")
				locations[0] = pieces[i][j]->GetIndexes();
			if (pieces[i][j] && pieces[i][j]->GetType() == "bK")
				locations[1] = pieces[i][j]->GetIndexes();
		}
	}
	return locations;
}

int Game::IsLegalMove(std::vector<sf::Vector2i> moves, sf::Vector2i move)
{
	for (auto m : moves) {
		if (m == move)
			return 1;
	}
	return 0;
}

void Game::HandleChecks()
{
	for (int i = 0; i < 2; i++)
	{
		whiteTurn = !whiteTurn;
		if (InCheck()) {
			if (whiteTurn) {
				sf::Vector2i wKLoc = GetKingsLocation()[0];
				pieces[wKLoc.x][wKLoc.y]->SetSpriteColor(sf::Color::Red);
			}
			else
			{
				sf::Vector2i bKLoc = GetKingsLocation()[1];
				pieces[bKLoc.x][bKLoc.y]->SetSpriteColor(sf::Color::Red);
			}

		}
		else
		{
			if (whiteTurn) {
				sf::Vector2i wKLoc = GetKingsLocation()[0];
				pieces[wKLoc.x][wKLoc.y]->SetSpriteColor(sf::Color::White);
			}
			else
			{
				sf::Vector2i bKLoc = GetKingsLocation()[1];
				pieces[bKLoc.x][bKLoc.y]->SetSpriteColor(sf::Color::White);
			}
		}
	}
	
}

void Game::checkmateOrStalemate()
{
	int check_moves = 1;
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j] && pieces[i][j]->IsWhite() == whiteTurn) {
				auto moves = GetLegalMoves(pieces[i][j]);
				if (moves.size()) {
					check_moves = 0;
				}
			}
				
		}
	}
	if (check_moves) {
		if (InCheck())
			checkmate = 1;
		else
			stalemate = 1;
	}
}










