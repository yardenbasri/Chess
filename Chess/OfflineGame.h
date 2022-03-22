#pragma once
#include "Game.h"
class OfflineGame :
    public Game
{   
public:
    OfflineGame(); // Ctor.
    void Init(); // loop function.
    void UpdateEvents(); // hadles all the events.
    void Update();
    void Print(); // print all needed sprites to the screen.
    void HandleClickes(sf::Vector2i indexes); // gets the indexes of the mouse position when it was clicked and handles all the clickes.
private:
    sf::Vector2i sqSelected; // save the indexes of the last selected squre.
};

