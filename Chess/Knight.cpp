#include "Knight.h"

Knight::Knight(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type)
{
	this->position = position;
	this->piece.setTexture(pieceTex);
	//this->piece.setScale(0.78125f, 0.78125f);
	//this->piece.setScale(1.2f, 1.2f);
	this->piece.setPosition(position.x, position.y);
	this->isWhite = isWhite;
	this->indexes = indexes;
	this->type = type;
	this->pieceMoved = 0;
}

int Knight::IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	std::vector<sf::Vector2i> possibleMoves = { {this->indexes.x + 2, this->indexes.y + 1}, {this->indexes.x + 2, this->indexes.y - 1 },
	{this->indexes.x - 2, this->indexes.y + 1}, {this->indexes.x - 2, this->indexes.y - 1},
	{this->indexes.x + 1, this->indexes.y + 2}, {this->indexes.x - 1, this->indexes.y + 2},
	{this->indexes.x + 1, this->indexes.y - 2}, {this->indexes.x - 1, this->indexes.y - 2} };

	for (auto move : possibleMoves) {
		if (move == dest)
		{
			return OppositeColors(dest, pieces);
		}
	}
	

	return 0;
}

std::vector<sf::Vector2i> Knight::GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	std::vector<sf::Vector2i> possibleMoves = { {this->indexes.x + 2, this->indexes.y + 1}, {this->indexes.x + 2, this->indexes.y - 1 },
	{this->indexes.x - 2, this->indexes.y + 1}, {this->indexes.x - 2, this->indexes.y - 1},
	{this->indexes.x + 1, this->indexes.y + 2}, {this->indexes.x - 1, this->indexes.y + 2},
	{this->indexes.x + 1, this->indexes.y - 2}, {this->indexes.x - 1, this->indexes.y - 2} };
	for (int i = possibleMoves.size()-1; i >= 0; i--)
	{
		if (!InBoard(possibleMoves[i]) || !OppositeColors(possibleMoves[i], pieces))
			possibleMoves.erase(possibleMoves.begin() + i);
	}
	return possibleMoves;
}

