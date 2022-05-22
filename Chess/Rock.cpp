#include "Rock.h"




Rock::Rock(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type)
{
	this->position = position;
	this->piece.setTexture(pieceTex);
	//this->piece.setScale(0.78125f, 0.78125f);
	//this->piece.setScale(1.2f, 1.2f);
	this->piece.setPosition(position.x, position.y);
	this->isWhite = isWhite;
	this->indexes = this->first_indexes =  indexes;
	this->type = type;
	this->pieceMoved = 0;
}

int Rock::IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	if (this->indexes != this->first_indexes)
		pieceMoved = 1;
	sf::Vector2i temp = indexes;
	int emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x++;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			if (OppositeColors(dest, pieces)) {
				return 1;
			}
			else return 0;
			 
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x--;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			if (OppositeColors(dest, pieces)) {
				return 1;
			}
			else return 0;
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.y--;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			if (OppositeColors(dest, pieces)) {
				return 1;
			}
			else return 0;
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.y++;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			if (OppositeColors(dest, pieces)) {
				return 1;
			}
			else return 0;
		}
	}
	return 0;
}

std::vector<sf::Vector2i> Rock::GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	if (this->indexes != this->first_indexes)
		pieceMoved = 1;
	std::vector<sf::Vector2i> possibleMoves;
	UpdatePossibleMoves(&possibleMoves, 1, 0, pieces, DIMENSIONS);
	UpdatePossibleMoves(&possibleMoves, -1, 0, pieces, DIMENSIONS);
	UpdatePossibleMoves(&possibleMoves, 0, 1, pieces, DIMENSIONS);
	UpdatePossibleMoves(&possibleMoves, 0, -1, pieces, DIMENSIONS);
	return possibleMoves;
}


