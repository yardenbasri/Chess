#include "King.h"

King::King(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type)
{
	this->position = position;
	this->piece.setTexture(pieceTex);
	//this->piece.setScale(1.2f, 1.2f);
	this->piece.setPosition(position.x, position.y);
	this->isWhite = isWhite;
	this->indexes = indexes;
	this->first_indexes = indexes;
	this->type = type;
	this->pieceMoved = 0;
}

int King::IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	if (this->indexes != this->first_indexes)
		pieceMoved = 1;

	std::vector<sf::Vector2i> possibleMoves = {
		{this->indexes.x + 1, this->indexes.y},
		{this->indexes.x + 1, this->indexes.y + 1},
		{this->indexes.x + 1, this->indexes.y - 1},
		{this->indexes.x - 1, this->indexes.y},
		{this->indexes.x - 1, this->indexes.y + 1},
		{this->indexes.x - 1, this->indexes.y - 1},
		{this->indexes.x, this->indexes.y + 1},
		{this->indexes.x, this->indexes.y - 1},
	};
	for (auto move : possibleMoves) {
		if (move == dest)
		{
			if (OppositeColors(dest, pieces)) {
				return 1;
			}
			else return 0;
		}
	}
	possibleMoves.clear();
	possibleMoves.push_back({ this->indexes.x, this->indexes.y + 2 });
	possibleMoves.push_back({ this->indexes.x, this->indexes.y - 2 });
	if (possibleMoves[0] == dest) // check right side castling
	{
		if (!pieces[this->indexes.x][this->indexes.y + 1] && !pieces[this->indexes.x][this->indexes.y + 2]
			&& !pieceMoved && pieces[this->indexes.x][this->indexes.y + 3] && !pieces[this->indexes.x][this->indexes.y + 3]->PieceMoved())
			return 2;
	}
	if (possibleMoves[1] == dest) // check left side castling
	{
		if (!pieces[this->indexes.x][this->indexes.y - 1] && !pieces[this->indexes.x][this->indexes.y - 2] && !pieces[this->indexes.x][this->indexes.y - 3]
			&& !pieceMoved && pieces[this->indexes.x][this->indexes.y - 4] && !pieces[this->indexes.x][this->indexes.y - 4]->PieceMoved())
			return 2;
	}
	
	return 0;
}






