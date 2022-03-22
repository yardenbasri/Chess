#include <SFML/Graphics.hpp>
#include "Game.h"
#include "OfflineGame.h"
int main()
{
	Game* game = new OfflineGame();
	game->Start();
}