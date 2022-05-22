#pragma once
#include "Game.h"
class OfflineGame :
    public Game
{   
public:
    OfflineGame(); // Ctor.
    void Init(); // loop function.
    void Update();
    void Print(); // print all needed sprites to the screen.
    bool PlayerTurn(sf::Vector2i indexes);

   
};

