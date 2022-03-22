#include "Piece.h"



Piece::Piece(sf::Texture& pieceTex, sf::Vector2f position, int isWhite, sf::Vector2i indexes, std::string type)
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

Piece::Piece()
{
}




void Piece::Print(sf::RenderWindow* window)
{
	window->draw(piece);
}


sf::Sprite Piece::GetSprite()
{
	return this->piece;
}
sf::Vector2f Piece::GetPosition()
{
	return this->piece.getPosition();
}
void Piece::SetPosition(sf::Vector2f position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->piece.setPosition(this->position);
}

void Piece::SetIndexes(sf::Vector2i indexes)
{
	this->indexes = indexes;
}

void Piece::SetSpriteColor(sf::Color color)
{
	this->piece.setColor(color);
}

void Piece::SetLastMove(sf::Vector2i lastMove[2])
{
	this->lastMove[0] = lastMove[0];
	this->lastMove[1] = lastMove[1];
}

int Piece::OppositeColors(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	if (pieces[dest.x][dest.y]) {
		if (pieces[dest.x][dest.y]->GetType()[0] !=
			pieces[indexes.x][indexes.y]->GetType()[0]) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 1;
	}
}

int Piece::InRange(sf::Vector2i move)
{
	return move.x >= 0 && move.x < 8 && move.y >= 0 && move.y < 8;
}

int Piece::PieceMoved()
{
	return this->pieceMoved;
}




std::vector<sf::Vector2i> Piece::GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	std::vector<sf::Vector2i> possibleMoves;
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[indexes.x][indexes.y]->IsPossibleMove(sf::Vector2i(i, j), pieces))
			{
				possibleMoves.push_back(sf::Vector2i(i, j));
			}
		}
	}
	return possibleMoves;
}





std::vector<sf::Vector2i> Piece::GetCastlingMoves(Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	return std::vector<sf::Vector2i>();
}




int Piece::IsWhite()
{
	return this->isWhite;
}

sf::Vector2i Piece::GetIndexes()
{
	return this->indexes;
}

std::string Piece::GetType()
{
	return this->type;
}






