#include "Menu.h"



Menu::Menu()
{
	window = new sf::RenderWindow(sf::VideoMode(SCREEN_SIZE, SCREEN_SIZE), "Menu");
	btnColor = sf::Color(255, 248, 220);
}

void Menu::Start()
{
	LoadData();
	LoadSprites();
	while (window->isOpen())
	{
		UpdateEvents();
		Print();
	}
	
}

void Menu::LoadData()
{
	buttons.push_back(new Button("Play Offline", 20, { SCREEN_SIZE / 4, SCREEN_SIZE / 16 }, sf::Color::Black, btnColor, { SCREEN_SIZE * 0.15, SCREEN_SIZE * 0.01 }));
	buttons.push_back(new Button("Play Ai", 20, { SCREEN_SIZE / 4, SCREEN_SIZE / 16 }, sf::Color::Black, btnColor, { SCREEN_SIZE * 0.85, SCREEN_SIZE * 0.01 }));
	chessBGTex.loadFromFile("Assets/ChessBG1.jpg");
	chessLogoTex.loadFromFile("Assets/ChessLogo1.png");
}

void Menu::LoadSprites()
{
	chessBG.setTexture(chessBGTex);
	chessLogo.setTexture(chessLogoTex);
	chessLogo.setPosition({ SCREEN_SIZE * 0.5f - chessLogo.getGlobalBounds().width / 2 }, { SCREEN_SIZE * 0.05 });
}

void Menu::UpdateEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
		if (event.type == sf::Event::MouseButtonPressed)
			for (int i = 0; i < buttons.size(); i++)
				if ((buttons[i]->IsMouseHover(*window)))
				{
					window->close();
					if (i == States::OFFLINE) {
						game = new OfflineGame();
						game->Start();
					}
					else if (i == States::AI) {
						game = new AiGame();
						game->Start();
					}
						
					
				}
		if (event.type == sf::Event::MouseMoved)
			for (auto btn : buttons)
				btn->HandleMouseHover(*window);
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

	}
}

void Menu::Print()
{
	window->clear();
	window->draw(chessBG);
	window->draw(chessLogo);
	for (auto btn : buttons)
		btn->DrawTo(*window);
	window->display();
}

