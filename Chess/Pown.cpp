#include "Pown.h"



Pown::Pown(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type)
{
	this->position = position;
	this->piece.setTexture(pieceTex);
	//this->piece.setScale(1.2f, 1.2f);
	this->piece.setPosition(position.x, position.y);
	this->isWhite = isWhite;
	this->indexes = indexes;
	this->type = type;
	this->pieceMoved = 0;

}

int Pown::IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	sf::Vector2i possibleMoves[4];
	if (isWhite)
	{
		possibleMoves[0] = { this->indexes.x - 1, this->indexes.y + 1 };
		possibleMoves[1] = { this->indexes.x - 1, this->indexes.y - 1 };
		possibleMoves[2] = { this->indexes.x - 1, this->indexes.y };
		possibleMoves[3] = { this->indexes.x - 2, this->indexes.y };
		if (this->indexes.x != 6)
			pieceMoved = 1;
	}
	else  
	{
		possibleMoves[0] = { this->indexes.x + 1, this->indexes.y + 1 };
		possibleMoves[1] = { this->indexes.x + 1, this->indexes.y - 1 };
		possibleMoves[2] = { this->indexes.x + 1, this->indexes.y };
		possibleMoves[3] = { this->indexes.x + 2, this->indexes.y };
		if (this->indexes.x != 1)
			pieceMoved = 1;
		
	}

	for (int i = 0; i < 2; i++)
	{
		if (possibleMoves[i] == dest)
		{
			if (pieces[dest.x][dest.y]) // check if dest squre is not empty
			{
				if (pieces[indexes.x][indexes.y]->IsWhite() != pieces[dest.x][dest.y]->IsWhite())
				{
					//pieceMoved = 1;
					return 1;
				}
				
			}
			else
			{
				// check for potential en passent:
				return IsEnPassent(dest);
			}
		}
	}
	for (int i = 2; i < 4; i++)
	{
		if (possibleMoves[i] == dest)
		{
			if (i == 2)
			{
				if (!pieces[dest.x][dest.y]) {
					return 1; // returns 1 only if dest squre is empty.
				}
			}
			else
			{
				if (!pieceMoved)
				{
					if(isWhite)
						return !pieces[dest.x][dest.y] && !pieces[dest.x + 1][dest.y]; 
					else
						return !pieces[dest.x][dest.y] && !pieces[dest.x - 1][dest.y];
				}
			}
		}
	}
	return 0;
}


std::vector<sf::Vector2i> Pown::GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	std::vector<sf::Vector2i> moves;

	sf::Vector2i possibleMoves[4];
	if (isWhite)
	{
		possibleMoves[0] = { this->indexes.x - 1, this->indexes.y + 1 };
		possibleMoves[1] = { this->indexes.x - 1, this->indexes.y - 1 };
		possibleMoves[2] = { this->indexes.x - 1, this->indexes.y };
		possibleMoves[3] = { this->indexes.x - 2, this->indexes.y };
	}
	else
	{
		possibleMoves[0] = { this->indexes.x + 1, this->indexes.y + 1 };
		possibleMoves[1] = { this->indexes.x + 1, this->indexes.y - 1 };
		possibleMoves[2] = { this->indexes.x + 1, this->indexes.y };
		possibleMoves[3] = { this->indexes.x + 2, this->indexes.y };

	}
	for (auto move : possibleMoves) {
		if (pieces[indexes.x][indexes.y]->IsPossibleMove(move, pieces))
		{
			moves.push_back(move);
		}
	}

	return moves;
}

int Pown::IsEnPassent(sf::Vector2i dest)
{
	printf("(%d, %d) -> (%d, %d)\n", lastMove[0].x, lastMove[0].y, lastMove[1].x, lastMove[1].y);
	if (isWhite)
	{
		if (this->indexes.x != 3)
			return 0;
		if (lastMove[0] == sf::Vector2i(dest.x - 1, dest.y) && lastMove[1] == sf::Vector2i(dest.x+1, dest.y))
		{
			return 1;
		}
	}
	else
	{
		if (this->indexes.x != 4)
			return 0;
		if (lastMove[0] == sf::Vector2i(dest.x + 1, dest.y) && lastMove[1] == sf::Vector2i(dest.x - 1, dest.y))
		{
			return 1;
		}
	}
	return 0;
}


