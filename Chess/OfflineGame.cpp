#include "OfflineGame.h"

OfflineGame::OfflineGame()
{
	mReleased = 0;
	sqSelected = { -1,-1 };
	whiteTurn = 1;
	isUndoPossible = false;
	mReleased = true;
	checkmate = 0;
	stalemate = 0;
	gameOver = 0;
	promotionChoice = 0;
	inCheck = false;
	promotionTypeVec.push_back(QUEEN);
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


void OfflineGame::Update()
{
	
	if (!gameOver && !gamePaused)
		UpdateBoard();
	else
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttons[0]->IsMouseHover(*window))
			CleanBoard();
	if (gamePaused)
	{
		for (auto btn : buttons)
			btn->HandleMouseHover(*window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (buttons[0]->IsMouseHover(*window)) {
				CleanBoard();
				gamePaused = false;
			}
			else if (buttons[1]->IsMouseHover(*window)) {
				gamePaused = false;
			}
			else if (buttons[2]->IsMouseHover(*window)) {
				window->close();
			}

		}

	}
	if (checkmate) {
		gameOver = 1;
		if (whiteTurn)
			text_msg.setString("Checkmate\nBlack won !");
		else
			text_msg.setString("Checkmate\nWhite won !");
	}
	if (stalemate)
	{
		gameOver = 1;
		text_msg.setString("Stalemate !");
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		CleanBoard(); // restart the game.
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
	if (gamePaused)
		for (auto btn : buttons)
			btn->DrawTo(*window);

	if (gameOver)
	{
		//window->clear();
		window->draw(text_msg);
		buttons[0]->DrawTo(*window);
		buttons[0]->HandleMouseHover(*window);
	}
	window->display();
}

bool OfflineGame::PlayerTurn(sf::Vector2i indexes)
{
	return pieces[indexes.x][indexes.y]->IsWhite() == whiteTurn;
}



