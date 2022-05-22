#include "Game.h"

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_SIZE, SCREEN_SIZE), "Yarden`s Chess");	
	isUndoPossible = false;
	darkSqure = darkSqures[0];
	lightSqure = lightSqures[0];
	markedSqure = sf::Color(255, 255, 200);
	lastMoveVec.push_back(std::pair<sf::Vector2i, sf::Vector2i>({ -1,-1 }, { -1, -1 }));
	auto btnColor = sf::Color(200, 157, 124);
	buttons.push_back(new Button("Restart", 20, { SCREEN_SIZE / 2, SCREEN_SIZE / 8 }, sf::Color::Black, btnColor, { SCREEN_SIZE * 0.5, SCREEN_SIZE * 0.1 }));
	buttons.push_back(new Button("Resume", 20, { SCREEN_SIZE / 2, SCREEN_SIZE / 8 }, sf::Color::Black, btnColor, { SCREEN_SIZE * 0.5, SCREEN_SIZE * 0.4 }));
	buttons.push_back(new Button("Exit", 20, { SCREEN_SIZE / 2, SCREEN_SIZE / 8 }, sf::Color::Black, btnColor, { SCREEN_SIZE * 0.5, SCREEN_SIZE * 0.7 }));

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
		piecesTex[piece].loadFromFile("Assets/pieces/" + piece + ".png");
	}
	
	arial_font.loadFromFile("Assets/arial.ttf");
	// letters and numbers text
	text_board.setFont(arial_font);
	text_board.setCharacterSize(16);
	text_board.setFillColor(sf::Color::Black);
	text_board.setOutlineThickness(1);
	text_board.setOutlineColor(sf::Color::White);
	// message text
	text_msg.setFont(arial_font);
	text_msg.setOutlineThickness(2);
	text_msg.setOutlineColor(sf::Color::Black);
	text_msg.setCharacterSize(90);
	text_msg.setFillColor(sf::Color(200, 157, 124));
	text_msg.setPosition(100, 350);
	// clear vectors
}

void Game::CreateBoard()
{
	sf::RectangleShape rectangle(sf::Vector2f(DELTA_POS, DELTA_POS));

	squres.clear();
	
	int mod = 0;
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			rectangle.setPosition((DELTA_POS)*j, (DELTA_POS)*i);
			if (j % 2 == mod)
			{
				rectangle.setFillColor(lightSqure);
				rectangle.setOutlineThickness(1);
				rectangle.setOutlineColor(sf::Color::Black);
			}
			else
			{
				rectangle.setFillColor(darkSqure);
				rectangle.setOutlineThickness(1);
				rectangle.setOutlineColor(sf::Color::Black);
			}
			squres.push_back(rectangle);

		}
		mod = !mod;
	}
}

void Game::LoadSprites()
{
	board.setTexture(boardTex);
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			std::string p = boardStatus[i][j];
			int isWhite = (p[0] == 'w');
			switch (p[1])
			{
			case 'p':
				pieces[i][j] = new Pown(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), isWhite, sf::Vector2i(i, j), p);
				break;
			case 'K':
				pieces[i][j] = new King(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), isWhite, sf::Vector2i(i, j), p);				
				break;
			case 'Q':
				pieces[i][j] = new Queen(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), isWhite, sf::Vector2i(i, j), p);
				break;
			case 'R':
				pieces[i][j] = new Rock(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), isWhite, sf::Vector2i(i,j), p);
				break;
			case 'B':
				pieces[i][j] = new Bishop(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), isWhite, sf::Vector2i(i, j), p);
				break;
			case 'N':
				pieces[i][j] = new Knight(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), isWhite, sf::Vector2i(i, j), p);
				break;

			default:
				pieces[i][j] = NULL;
				break;
			}
			
		}
	}
	
}

void Game::UpdateEvents()
{
	sf::Event event;

	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
		}
		if (event.type == sf::Event::MouseButtonReleased)
			mReleased = true;
		if (event.type == sf::Event::Resized)
		{
			// update the view to the new size of the window
			sf::View view;
			if (event.size.width < event.size.height)
			{
				sf::FloatRect visibleArea(0.f, 0.f, SCREEN_SIZE, (SCREEN_SIZE * event.size.height) / event.size.width);
				view = sf::View(visibleArea);
				window->setView(view);

			}
			else
			{
				sf::FloatRect visibleArea(0.f, 0.f, (SCREEN_SIZE * event.size.width) / event.size.height, SCREEN_SIZE);
				view = sf::View(visibleArea);
				window->setView(view);
			}
			view.setCenter(sf::Vector2f(SCREEN_SIZE / 2, SCREEN_SIZE / 2));
			window->setView(view);


		}
		if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			gamePaused = true;
		}
		
		if (event.type == sf::Event::KeyReleased)
			keyReleased = true;
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
			if (squres[i * DIMENSIONS + j].getGlobalBounds().contains(mPos))
			{
				return sf::Vector2i(i, j);
			}
		}
	}
	return sf::Vector2i(-1, -1);
}

void Game::UpdateBoard()
{
	auto mousePosition = sf::Mouse::getPosition(*window);
	auto mPos = window->mapPixelToCoords(mousePosition);
	sf::Vector2i indexes = Position2Indexes(mPos);
	if (promotionChoice)
		for (auto i : promotionIndexes)
		{
			if (i == indexes)
				MarkSqure(i, markedSqure);
			else
				MarkSqure(i, sf::Color::White);
		}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mReleased )
	{
		HandleClickes(indexes);
		mReleased = false;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mReleased)
	{
		sf::View view = window->getView();
		view.rotate(180.f);
		window->setView(view);
		RotatePieces();
		mReleased = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && isUndoPossible)
	{
		UndoMove(lastMoveVec.back().first, lastMoveVec.back().second);
		whiteTurn = !whiteTurn;
		HandleChecks();
		isUndoPossible = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && keyReleased)
	{
		colorIndex++;
		if (colorIndex == NUM_OF_COLORS)
			colorIndex = 0;
		lightSqure = lightSqures[colorIndex];
		darkSqure = darkSqures[colorIndex];
		CreateBoard();
		keyReleased = false;
	}
}

void Game::DrawPromotionOptions()
{
	promotionIndexes.clear();
	promotionPieces.clear();
	promotionPiecesReplace.clear();
	int xDir = whiteTurn ? -1 : 1;
	auto temp = lastMoveVec.back().second;
	int isWhite = pieces[temp.x][temp.y]->IsWhite();
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 1:
			if (pieces[temp.x][temp.y])
				promotionPiecesReplace.push_back(pieces[temp.x][temp.y]);
			else
				promotionPiecesReplace.push_back(nullptr);
			if (isWhite)
				pieces[temp.x][temp.y] = new Knight(piecesTex["wN"], Indexes2Position(temp), 1, temp, "wN");
			else
				pieces[temp.x][temp.y] = new Knight(piecesTex["bN"], Indexes2Position(temp), 0, temp, "bN");
			break;
		case 2:
			if (pieces[temp.x][temp.y])
				promotionPiecesReplace.push_back(pieces[temp.x][temp.y]);
			else
				promotionPiecesReplace.push_back(nullptr);
			if (isWhite)
				pieces[temp.x][temp.y] = new Rock(piecesTex["wR"], Indexes2Position(temp), 1, temp, "wR");
			else
				pieces[temp.x][temp.y] = new Rock(piecesTex["bR"], Indexes2Position(temp), 0, temp, "bR");
			break;
		case 3:
			if (pieces[temp.x][temp.y])
				promotionPiecesReplace.push_back(pieces[temp.x][temp.y]);
			else
				promotionPiecesReplace.push_back(nullptr);
			if (isWhite)
				pieces[temp.x][temp.y] = new Bishop(piecesTex["wB"], Indexes2Position(temp), 1, temp, "wB");
			else
				pieces[temp.x][temp.y] = new Bishop(piecesTex["bB"], Indexes2Position(temp), 0, temp, "bB");
			break;
		default:
			break;
		}
		promotionIndexes.push_back(temp);
		if (!i) {
			promotionPieces.push_back(pieces[temp.x][temp.y]);
			promotionPiecesReplace.push_back(nullptr);
		}
		else
			promotionPieces.push_back(pieces[temp.x][temp.y]);
		MarkSqure(temp, sf::Color::White);
		temp.x += xDir;
	}
}

void Game::HandlePromotionOptions(sf::Vector2i indexes)
{
	int found = 0;
	for (int i = 0; i < promotionIndexes.size(); i++)
	{
		if (promotionIndexes[i] == indexes)
		{
			if (i > 0)
				RegularMove(indexes, promotionIndexes[0]);
			found = 1;
			break;
		}

	}
	if (found)
	{
		for (int i = 0; i < 4; i++)
		{
			cleanSqure(promotionIndexes[i]);
			if (!i) continue;
			pieces[promotionIndexes[i].x][promotionIndexes[i].y] = promotionPiecesReplace[i];
		}
		promotionChoice = 0;
		isPromInit = 1;
		HandleChecks();
		checkmateOrStalemate();
		isUndoPossible = true;
		
	}
}

std::string Game::TranslateMove(std::pair<sf::Vector2i, sf::Vector2i> move)
{
	char letter = 'a', number = '8';
	letter += move.first.y;
	number -= move.first.x;
	std::string str;
	str.push_back(letter);
	str.push_back(number);
	letter = 'a';
	number = '8';
	letter += move.second.y;
	number -= move.second.x;
	str.push_back(letter);
	str.push_back(number);
	return str;
}

void Game::HandleClickes(sf::Vector2i indexes)
{
	if (promotionChoice)
	{
		HandlePromotionOptions(indexes);
	}
	else if (indexes.x != -1 && (pieces[indexes.x][indexes.y] || playerClickes.size())) // check if the first click was not an empty squre
	{
		if (sqSelected == indexes && playerClickes.size() == 1) // check if the same squre is pressed twice.
		{
			if (pieces[indexes.x][indexes.y]->IsWhite())
				cleanSqure(sqSelected);
			else
				cleanSqure(sqSelected);
			possibleCircles.clear();
			sqSelected = { -1,-1 };
			playerClickes.clear();
		}
		else
		{
			sqSelected = indexes;
			playerClickes.push_back(sqSelected);
			MarkSqure(sqSelected, markedSqure);
			if (playerClickes.size() == 1 && PlayerTurn(playerClickes[0])) {
				legalMoves = GetLegalMoves(pieces[playerClickes[0].x][playerClickes[0].y]);
				possibleCircles = GetPossibleCircles(legalMoves);
			}

			if (playerClickes.size() == 2) // check if two different squres has been pressed.
			{
				auto it = std::find(legalMoves.begin(), legalMoves.end(), playerClickes[1]); // 'it' will be the end of the vector if the move will not be found
				bool legalMove = (it != legalMoves.end());
				if (PlayerTurn(playerClickes[0]) && legalMove)
				{
					lastType = pieces[playerClickes[0].x][playerClickes[0].y]->GetType();
					Move(playerClickes[0], playerClickes[1]);
					UpdatesAfterMove();
				}
				else
				{
					if (whiteTurn) {
						cleanSqure(playerClickes[0]);
						cleanSqure(playerClickes[1]);
					}
					else
					{
						cleanSqure(playerClickes[0]);
						cleanSqure(playerClickes[1]);
					}
					possibleCircles.clear();
					playerClickes.clear();
				}

			}
		}
	}

}

void Game::RegularMove(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter)
{
	pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(indexesAfter);
	pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(indexesAfter));
	pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
	pieces[indexesBefore.x][indexesBefore.y] = NULL;
}

void Game::MarkSqure(sf::Vector2i indexes, sf::Color color)
{
	squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(color);
}

void Game::cleanSqure(sf::Vector2i indexes)
{
	if (indexes.x % 2 == 0 && indexes.y % 2 == 0 || indexes.x % 2 == 1 && indexes.y % 2 == 1)
	{
		squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(lightSqure);
	}
	else
	{
		squres[indexes.x * DIMENSIONS + indexes.y].setFillColor(darkSqure);
	}
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
				pieces[i][j] = new Pown(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'K':
				pieces[i][j] = new King(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'Q':
				pieces[i][j] = new Queen(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
				break;
			case 'R':
				pieces[i][j] = new Rock(piecesTex[p], Indexes2Position(sf::Vector2i(i, j)), p[0] == 'w', sf::Vector2i(i, j), p);
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
	if (sqSelected != sf::Vector2i(-1, -1)) {
		cleanSqure(sqSelected);
		sqSelected = { -1,-1 };
	}
	playerClickes.clear();
	possibleCircles.clear();
	whiteTurn = 1;
	checkmate = 0;
	stalemate = 0;
	gameOver = 0;


}

void Game::Castling(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter)
{
	RegularMove(indexesBefore, indexesAfter);
	if (indexesAfter.y > indexesBefore.y)//	king castling
	{
		pieces[indexesAfter.x][indexesAfter.y + 1]->SetIndexes(sf::Vector2i(indexesAfter.x, indexesAfter.y - 1));
		pieces[indexesAfter.x][indexesAfter.y + 1]->SetPosition(Indexes2Position(sf::Vector2i(indexesAfter.x, indexesAfter.y - 1)));
		pieces[indexesAfter.x][indexesAfter.y - 1] = pieces[indexesAfter.x][indexesAfter.y + 1];
		pieces[indexesAfter.x][indexesAfter.y + 1] = NULL;
		moveTypeVec.push_back(KING_CASTLING);
	}
	else  // queen castling
	{
		pieces[indexesAfter.x][indexesAfter.y - 2]->SetIndexes(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1));
		pieces[indexesAfter.x][indexesAfter.y - 2]->SetPosition(Indexes2Position(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1)));
		pieces[indexesAfter.x][indexesAfter.y + 1] = pieces[indexesAfter.x][indexesAfter.y - 2];
		pieces[indexesAfter.x][indexesAfter.y - 2] = NULL;
		moveTypeVec.push_back(QUEEN_CASTLING);
	}
}

void Game::Promotion(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter, bool isWhite)
{
	if(isWhite)
		switch (promotionTypeVec.back())
		{
		case promotionTypes::QUEEN:
			pieces[indexesAfter.x][indexesAfter.y] = new Queen(piecesTex["wQ"], Indexes2Position(indexesAfter), 1, indexesAfter, "wQ");
			break;
		case promotionTypes::KNIGHT:
			pieces[indexesAfter.x][indexesAfter.y] = new Knight(piecesTex["wN"], Indexes2Position(indexesAfter), 1, indexesAfter, "wN");
			break;
		case promotionTypes::ROCK:
			pieces[indexesAfter.x][indexesAfter.y] = new Rock(piecesTex["wR"], Indexes2Position(indexesAfter), 1, indexesAfter, "wR");
			break;
		case promotionTypes::BISHOP:
			pieces[indexesAfter.x][indexesAfter.y] = new Bishop(piecesTex["wB"], Indexes2Position(indexesAfter), 1, indexesAfter, "wB");
			break;
		}
	else
		switch (promotionTypeVec.back())
		{
		case promotionTypes::QUEEN:
			pieces[indexesAfter.x][indexesAfter.y] = new Queen(piecesTex["bQ"], Indexes2Position(indexesAfter), 0, indexesAfter, "bQ");
			break;
		case promotionTypes::KNIGHT:
			pieces[indexesAfter.x][indexesAfter.y] = new Knight(piecesTex["bN"], Indexes2Position(indexesAfter), 0, indexesAfter, "bN");
			break;
		case promotionTypes::ROCK:
			pieces[indexesAfter.x][indexesAfter.y] = new Rock(piecesTex["bR"], Indexes2Position(indexesAfter), 0, indexesAfter, "bR");
			break;
		case promotionTypes::BISHOP:
			pieces[indexesAfter.x][indexesAfter.y] = new Bishop(piecesTex["bB"], Indexes2Position(indexesAfter), 0, indexesAfter, "bB");
			break;
		}
	pieces[indexesBefore.x][indexesBefore.y] = NULL;
}

void Game::EnPassent(bool isWhite, sf::Vector2i indexesBefore, sf::Vector2i indexesAfter)
{
	pieces[indexesBefore.x][indexesBefore.y]->SetIndexes(indexesAfter);
	pieces[indexesBefore.x][indexesBefore.y]->SetPosition(Indexes2Position(indexesAfter));
	pieces[indexesAfter.x][indexesAfter.y] = pieces[indexesBefore.x][indexesBefore.y];
	pieces[indexesBefore.x][indexesBefore.y] = NULL;
	if (isWhite) {
		pieces[indexesAfter.x + 1][indexesAfter.y] = NULL; // delete the pown eaten.
	}
	else {
		pieces[indexesAfter.x - 1][indexesAfter.y] = NULL; // delete the pown eaten.
	}
		

}

int Game::IsCastlingLegal(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter)
{
	if (abs(indexesAfter.y - indexesBefore.y) == 2) // check if the move was castling.
	{
		if (InCheck())
			return 0;
		if (indexesAfter.y > indexesBefore.y)  // king castling
		{
			if (!squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y - 1)))
				return 1;
		}
		else if (!squreUnderAttack(sf::Vector2i(indexesAfter.x, indexesAfter.y + 1))) //queen castling
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

void Game::Move(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter)
{
	lastTypeVec.push_back(pieces[indexesBefore.x][indexesBefore.y]->GetType());
	lastMoveVec.push_back(std::pair<sf::Vector2i, sf::Vector2i>(indexesBefore, indexesAfter));
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j]) pieces[i][j]->SetLastMove(lastMoveVec.back());
		}
	}
	pieceMovedVec.push_back(pieces[indexesBefore.x][indexesBefore.y]->PieceMoved());
	Piece* pieceCaptured = pieces[indexesAfter.x][indexesAfter.y];
	capturedSqure = pieces[indexesAfter.x][indexesAfter.y];
	// check for special moves : 
	if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "bp" && indexesAfter.x == 7)  // check for bp promotion.
	{
		Promotion(indexesBefore, indexesAfter, false);
		moveTypeVec.push_back(moveTypes::PROMOTION);
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "wp" && indexesAfter.x == 0)  // check for wp promotion.
	{
		Promotion(indexesBefore, indexesAfter, true);
		moveTypeVec.push_back(PROMOTION);
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "wp" &&
		indexesBefore.x == 3 && indexesAfter.y != indexesBefore.y && !pieces[indexesAfter.x][indexesAfter.y]) // check white en passent
	{
		pieceCaptured = pieces[indexesAfter.x + 1][indexesAfter.y];
		EnPassent(true, indexesBefore, indexesAfter);
		moveTypeVec.push_back(WHITE_ENPASSENT);
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType() == "bp" && indexesBefore.x == 4 &&
		indexesAfter.y != indexesBefore.y && !pieces[indexesAfter.x][indexesAfter.y])  // check black en passent
	{
		pieceCaptured = pieces[indexesAfter.x - 1][indexesAfter.y];
		EnPassent(false, indexesBefore, indexesAfter);
		moveTypeVec.push_back(BLACK_ENPASSENT);
	}
	else if (pieces[indexesBefore.x][indexesBefore.y]->GetType()[1] == 'K' && abs(indexesAfter.y-indexesBefore.y) == 2 ) // check castling
	{
		Castling(indexesBefore, indexesAfter);
		
	}
	else // A regular move :
	{
		RegularMove(indexesBefore, indexesAfter);
		moveTypeVec.push_back(REGULARMOVE);
	}
	capturedPieces.push_back(pieceCaptured);
	//whiteTurn = !whiteTurn;
	
}

void Game::UndoMove(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter)
{
	lastMoveVec.pop_back();
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j]) pieces[i][j]->SetLastMove(lastMoveVec.back());
		}
	}
	RegularMove(indexesAfter, indexesBefore);
	pieces[indexesBefore.x][indexesBefore.y]->setPieceMoved(pieceMovedVec.back());
	pieceMovedVec.pop_back();
	auto t = moveTypeVec.back();
	switch (t)
	{
	case(moveTypes::PROMOTION):
	{
		std::string type = lastTypeVec[lastTypeVec.size() - 1];
		pieces[indexesBefore.x][indexesBefore.y] = new Pown(piecesTex[type], // create a new pown
			Indexes2Position(sf::Vector2i(indexesBefore.x, indexesBefore.y)),
			type[0] == 'w', sf::Vector2i(indexesBefore.x, indexesBefore.y), type);
		break;
	}
	case(moveTypes::WHITE_ENPASSENT):
	{
		pieces[indexesAfter.x + 1][indexesAfter.y] = capturedPieces[capturedPieces.size() - 1];
		break;
	}
	case(moveTypes::BLACK_ENPASSENT):
	{
		pieces[indexesAfter.x - 1][indexesAfter.y] = capturedPieces[capturedPieces.size() - 1];
		break;
	}
	case(moveTypes::KING_CASTLING):
	{
		auto before = sf::Vector2i(indexesAfter.x, indexesAfter.y - 1);
		auto after = sf::Vector2i(indexesAfter.x, indexesAfter.y + 1);
		RegularMove(before, after);
		pieces[after.x][after.y]->setPieceMoved(false);
		break;
	}
	case(moveTypes::QUEEN_CASTLING):
	{
		auto before = sf::Vector2i(indexesAfter.x, indexesAfter.y + 1);
		auto after = sf::Vector2i(indexesAfter.x, indexesAfter.y - 2);
		RegularMove(before, after);
		pieces[after.x][after.y]->setPieceMoved(false);
		break;
	}
		
	}

	if(t != moveTypes::WHITE_ENPASSENT && t != moveTypes::BLACK_ENPASSENT)
		pieces[indexesAfter.x][indexesAfter.y] = capturedPieces.back();
	capturedPieces.pop_back();
	moveTypeVec.pop_back();
	lastTypeVec.pop_back();
	//whiteTurn = !whiteTurn;
}

void Game::UpdatesAfterMove()
{
	if (moveTypeVec.back() == moveTypes::PROMOTION)
		promotionChoice = 1;
	cleanSqure(playerClickes[0]);
	cleanSqure(playerClickes[1]);
	possibleCircles.clear();
	playerClickes.clear();
	whiteTurn = !whiteTurn;
	if (promotionChoice) {
		if (isPromInit) {
			DrawPromotionOptions();
			isPromInit = 0;
		}
	}
	else
	{
		HandleChecks();
		checkmateOrStalemate();
		isUndoPossible = true;
	}
	
}
	

std::vector<sf::CircleShape> Game::GetPossibleCircles(std::vector<sf::Vector2i> possibleMoves)
{
	std::vector<sf::CircleShape> circles;
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Red);
	circle.setRadius(15);
	for (auto move : possibleMoves) {
		sf::Vector2f pos = Indexes2Position(move);
		pos.x += 23;
		pos.y += 25;
		circle.setPosition(pos);
		circles.push_back(circle);
	}
	return circles;
}

std::vector<sf::Vector2i> Game::GetLegalMoves(Piece* piece)
{
	// naive soluation:
	//std::vector<sf::Vector2i> moves = piece->GetPossibleMoves(pieces);
	//sf::Vector2i indexesBefore = piece->GetIndexes();
	//sf::Vector2i indexesAfter;	
	//
	//for (int i = moves.size() - 1; i >= 0; i--)
	//{
	//	// Do the move.
	//	indexesAfter = moves[i];
	//	Move(indexesBefore, indexesAfter);
	//
	//	if (InCheck())
	//		moves.erase(moves.begin() + i);
	//	UndoMove(indexesBefore, indexesAfter);
	//}
	//

	//
	//return moves;
	// advance solution:
	return piece->GetValidMoves(pieces, GetKingsLocation());
}

int Game::InBoard(sf::Vector2i indexes)
{

	return indexes.x >= 0 && indexes.x < DIMENSIONS && indexes.y >= 0 && indexes.y < DIMENSIONS;
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

	int dirX, dirY;
	int directions[][8] = { {-1,-1}, {-1, 1}, {1, -1}, {1, 1}, {0, -1}, {0, 1}, {1, 0}, {-1, 0} };
	sf::Vector2i temp = location;
	Piece* locPiece = pieces[location.x][location.y];
	int isWhite = locPiece->IsWhite();
	for (int i = 0; i < 8; i++)
	{
		dirX = directions[i][0];
		dirY = directions[i][1];
		temp.x += dirX;
		temp.y += dirY;
		int dis = 1;
		while (InBoard(temp))
		{
			if (pieces[temp.x][temp.y])
			{
				if (pieces[temp.x][temp.y]->IsWhite() != pieces[location.x][location.y]->IsWhite())
				{
					switch (pieces[temp.x][temp.y]->GetType()[1])
					{
					case 'Q':
						return 1;
					case 'B':
						if (i < 4)
							return 1;
						break;
					case 'R':
						if (i >= 4)
							return 1;
						break;
					case 'p':
						if (dis == 1)
							if (pieces[temp.x][temp.y]->IsWhite() && i >= 2 && i <= 3)
								return 1;
							else if (!pieces[temp.x][temp.y]->IsWhite() && i < 2)
								return 1;
						break;
					case 'K':
						if (dis == 1)
							return 1;
						break;
					} // end switch
					
				}
				break;
			}
			temp.x += dirX;
			temp.y += dirY;
			dis++;		
		}// end while
		temp = location;
	}
	// check for Knights :
	std::vector<sf::Vector2i> possibleMoves = { {location.x + 2, location.y + 1}, {location.x + 2, location.y - 1 },{location.x - 2, location.y + 1}, 
		{location.x - 2, location.y - 1}, {location.x + 1, location.y + 2}, {location.x - 1, location.y + 2},
	{location.x + 1, location.y - 2}, {location.x - 1, location.y - 2} };
	for (auto move : possibleMoves)
		if (InBoard(move) && pieces[move.x][move.y] && pieces[move.x][move.y]->GetType()[1] == 'N' && pieces[move.x][move.y]->IsWhite() != pieces[location.x][location.y]->IsWhite())
			return 1;
	return 0;


	// naive solution

	//for (int i = 0; i < DIMENSIONS; i++)
	//{
	//	for (int j = 0; j < DIMENSIONS; j++)
	//	{
	//		if (pieces[i][j] && pieces[i][j]->IsWhite() != pieces[location.x][location.y]->IsWhite()) // check if its white`s turn and the piece is white or if it` s black turn and the piece is black.
	//		{
	//			auto opponentPossibleMoves = pieces[i][j]->GetPossibleMoves(pieces);
	//			for (auto move : opponentPossibleMoves)
	//			{
	//				if (move == location) {
	//					return 1;
	//				}
	//			}
	//		}
	//	}
	//}
	//return 0;
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

void Game::HandleChecks()
{
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
		inCheck = true;
	}
	else
	{
		if (!whiteTurn) {
			sf::Vector2i wKLoc = GetKingsLocation()[0];
			pieces[wKLoc.x][wKLoc.y]->SetSpriteColor(sf::Color::White);
		}
		else
		{
			sf::Vector2i bKLoc = GetKingsLocation()[1];
			pieces[bKLoc.x][bKLoc.y]->SetSpriteColor(sf::Color::White);
		}
		inCheck = false;
	}
	
}

void Game::checkmateOrStalemate()
{
	auto moves = AllLegalMoves(whiteTurn);
	if (moves.empty()) {
		if (inCheck)
			checkmate = 1;
		else
			stalemate = 1;
	}
}

void Game::DrawNumbersLetters(sf::RenderWindow* window)
{
	sf::Vector2f pos = { 0.1*DELTA_POS , SCREEN_SIZE - 22 };
	std::string letters[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	text_board.setPosition(pos);
	for (auto letter : letters)
	{
		text_board.setString(letter);
		window->draw(text_board);
		pos.x += DELTA_POS;
		text_board.setPosition(pos);
	}
	pos = { 785 , SCREEN_SIZE - DELTA_POS };
	text_board.setPosition(pos);
	std::string numbers[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
	for (auto number : numbers)
	{
		text_board.setString(number);
		window->draw(text_board);
		pos.y -= DELTA_POS;
		text_board.setPosition(pos);
	}



}

void Game::RotatePieces()
{
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j])
				pieces[i][j]->RotatePiece(180.f);
		}
	}
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Game::AllLegalMoves(bool isWhite)
{
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> allMoves;
	std::vector<sf::Vector2i> moves;
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (!pieces[i][j]) continue;
			if (pieces[i][j]->IsWhite() == isWhite)
			{
				moves = GetLegalMoves(pieces[i][j]);
				for (auto move : moves)
					allMoves.push_back((std::pair<sf::Vector2i, sf::Vector2i>(sf::Vector2i(i, j), move)));
			}
				
		}
	}
	return allMoves;
}














