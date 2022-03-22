#include "Queen.h"


Queen::Queen(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type)
{
	this->position = position;
	this->piece.setTexture(pieceTex);
	//this->piece.setScale(1.2f, 1.2f);
	this->piece.setPosition(position.x, position.y);
	this->isWhite = isWhite;
	this->indexes = indexes;
	this->type = type;
}

int Queen::IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	sf::Vector2i temp = indexes;
	int emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x++;
		temp.y++;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			return OppositeColors(dest, pieces);
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x--;
		temp.y++;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			return OppositeColors(dest, pieces);
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x++;
		temp.y--;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			return OppositeColors(dest, pieces);
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x--;
		temp.y--;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			return OppositeColors(dest, pieces);
		}
	}
	temp = indexes;
	emptyBetween = 1;
	for (int j = 0; j < DIMENSIONS; j++)
	{
		temp.x++;
		if (pieces[temp.x][temp.y] && temp != dest) {
			emptyBetween = 0;
		}
		if (temp == dest && emptyBetween)
		{
			return OppositeColors(dest, pieces);
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
			return OppositeColors(dest, pieces);
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
			return OppositeColors(dest, pieces);
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
			return OppositeColors(dest, pieces);
		}
	}
	return 0;
}



