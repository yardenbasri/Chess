#pragma once
#include "Piece.h"
class Knight :
    public Piece
{
public:
    Knight(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type);
    int IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS]);
    std::vector<sf::Vector2i> GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS]);
};

