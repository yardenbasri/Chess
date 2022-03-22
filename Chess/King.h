#pragma once
#include "Piece.h"
class King :
    public Piece
{
public:
    King(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type);
    int IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS]);
    std::vector<sf::Vector2i> GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS]);
private:
    
};

