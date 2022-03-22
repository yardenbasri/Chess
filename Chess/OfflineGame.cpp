#include "OfflineGame.h"

OfflineGame::OfflineGame()
{
	mReleased = 0;
	sqSelected = { -1,-1 };
	whiteTurn = 1;
}
void OfflineGame::Init()
{
	while (window->isOpen())
	{
		UpdateEvents();
		Update();
		Print();
	}
}

void OfflineGame::UpdateEvents()
{
	sf::Event event;
	sf::Vector2f mPos;
	sf::Vector2<int> mousePosition;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
		}
		if (event.type == sf::Event::MouseButtonPressed && !gameOver)
		{	
			if (event.key.code == sf::Mouse::Left)
			{


				// before the fucking idiotic bag:
				/*mPos = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
				sf::Vector2i indexes = Position2Indexes(mPos);*/
				// After the bag :
				mousePosition = sf::Mouse::getPosition(*window);
				
				
				sf::Vector2i indexes = Position2Indexes(sf::Vector2f(mousePosition));
				if (!pieces[indexes.x][indexes.y] && playerClickes.size() == 0) // check if an empty squre was pressed.
					continue;
				
				HandleClickes(indexes);
				
				
				
			}
		} // end mouse button pressed

		if (event.type == sf::Event::Resized)
		{
			// update the view to the new size of the window
			sf::View view = window->getDefaultView();
			sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
			window->setView(sf::View(visibleArea));
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			CleanBoard(); // restart the game.
		}
		
		
	}
}

void OfflineGame::Update()
{
	if (flag != whiteTurn) {
		checkmateOrStalemate();
		flag = whiteTurn;
	}
		
	if (checkmate) {
		gameOver = 1;
		if (whiteTurn)
			text_msg.setString("Black won !\nPress space to restart.");
		else
			text_msg.setString("White won !\nPress space to restart.");
	}
	if (stalemate)
	{
		gameOver = 1;
		text_msg.setString("Stalemate !\nPress space to restart.");
	}

}



void OfflineGame::Print()
{
	window->clear();

	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			window->draw(squres[i * DIMENSIONS + j]);
		}
	}

	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j])
			{
				pieces[i][j]->Print(window);
			}
		}
	}
	DrawNumbersLetters(window);
	for (auto circle : possibleCircles) {
		window->draw(circle);
	}

	if (gameOver)
	{
		window->clear();
		window->draw(text_msg);
		
	}
	window->display();
}

void OfflineGame::HandleClickes(sf::Vector2i indexes)
{
	if (sqSelected == indexes && playerClickes.size() == 1) // check if the same squre is pressed twice.
	{
		cleanSqure(sqSelected);
		possibleCircles.clear();
		sqSelected = { -1,-1 };
		playerClickes.clear();
	}
	else
	{
		sqSelected = indexes;
		playerClickes.push_back(sqSelected);
		MarkSqure(sqSelected);
		if (playerClickes.size() == 1 && pieces[sqSelected.x][sqSelected.y]->IsWhite() == whiteTurn) {
			auto moves = GetLegalMoves(pieces[sqSelected.x][sqSelected.y]);
			for (int i = moves.size() - 1; i >= 0; i--)
			{
				playerClickes.push_back(moves[i]);
				if (!IsCastlingLegal())
					moves.erase(moves.begin() + i);
				playerClickes.pop_back();
			}
			
			possibleCircles = GetPossibleCircles(moves);
		}
			
	}
	if (playerClickes.size() == 2 ) // check if two different squres has been pressed.
	{
		auto moves = GetLegalMoves(pieces[playerClickes[0].x][playerClickes[0].y]);
		int legalMove = IsLegalMove(moves, playerClickes[1]);
		if (pieces[playerClickes[0].x][playerClickes[0].y]->IsWhite() == whiteTurn && legalMove && IsCastlingLegal())
		{
			cleanSqure(playerClickes[0]);
			cleanSqure(playerClickes[1]);
			possibleCircles.clear();
			

			if (legalMove)
				Move();
			

			lastMove[0] = playerClickes[0];
			lastMove[1] = playerClickes[1];
			for (int i = 0; i < DIMENSIONS; i++)
			{
				for (int j = 0; j < DIMENSIONS; j++)
				{
					if (pieces[i][j]) pieces[i][j]->SetLastMove(lastMove);
				}
			}
			playerClickes.clear();
			whiteTurn = !whiteTurn;
			HandleChecks();
			
		}
		else
		{
			cleanSqure(playerClickes[0]);
			cleanSqure(playerClickes[1]);
			possibleCircles.clear();
			playerClickes.clear();
		}
		
	}
	
}

