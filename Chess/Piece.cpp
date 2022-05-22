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

void Piece::SetLastMove(std::pair<sf::Vector2i, sf::Vector2i> lastMove)
{
	this->lastMove[0] = lastMove.first;
	this->lastMove[1] = lastMove.second;
}

void Piece::setPieceMoved(bool pieceMoved)
{
	this->pieceMoved = pieceMoved;
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

void Piece::RotatePiece(float rot)
{
	piece.rotate(rot);
}

int Piece::InBoard(sf::Vector2i indexes)
{
	return indexes.x >= 0 && indexes.x < DIMENSIONS && indexes.y >= 0 && indexes.y < DIMENSIONS;
}

void Piece::UpdatePossibleMoves(std::vector<sf::Vector2i> *possibleMoves, int xDir, int yDir, Piece* pieces[DIMENSIONS][DIMENSIONS], int limit)
{
	sf::Vector2i temp = indexes;
	temp.x += xDir;
	temp.y += yDir;
	while (InBoard(temp) && limit)
	{
		if (!pieces[temp.x][temp.y])
			possibleMoves->push_back(temp);
		else
		{
			if (OppositeColors(temp, pieces))
				possibleMoves->push_back(temp);
			break;
		}
		temp.x += xDir;
		temp.y += yDir;
		limit--;
	}
}

int Piece::squreUnderAttack(Piece* pieces[DIMENSIONS][DIMENSIONS], sf::Vector2i location)
{
	attackSqures.clear();
	pinSqures.clear();
	int dirX, dirY;
	int directions[][8] = { {-1,-1}, {-1, 1}, {1, -1}, {1, 1}, {0, -1}, {0, 1}, {1, 0}, {-1, 0} };
	sf::Vector2i temp = location;
	//Piece* locPiece = pieces[location.x][location.y];
	//int isWhite = locPiece->IsWhite();
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		dirX = directions[i][0];
		dirY = directions[i][1];
		temp.x += dirX;
		temp.y += dirY;
		int dis = 1;
		int checkForPins = 0; 
		int pinFound = 0;
		while (InBoard(temp))
		{
			if (pieces[temp.x][temp.y])
			{
				if (pieces[temp.x][temp.y]->IsWhite() != isWhite)
				{
					int check = checkForPins;
					switch (pieces[temp.x][temp.y]->GetType()[1])
					{
					case 'Q':
						if (!pinFound && checkForPins) {
							pinFound = 1;
							pinSqures.push_back(temp);
						}
						if (!checkForPins) {
							count++;
							attackSqures.push_back(temp);
						}
						break;
					case 'B':
						if (i < 4) {
							if (!pinFound && checkForPins) {
								pinFound = 1;
								pinSqures.push_back(temp);
							}
							if (!checkForPins) {
								count++;
								attackSqures.push_back(temp);
							}
						}
						break;
					case 'R':
						if (i >= 4) {
							if (!pinFound && checkForPins) {
								pinFound = 1;
								pinSqures.push_back(temp);
							}
							if (!checkForPins) {
								count++;
								attackSqures.push_back(temp);
							}
						}
						break;
					case 'p':
						if (dis == 1)
							if (pieces[temp.x][temp.y]->IsWhite() && i >= 2 && i <= 3) {
								if (!checkForPins) {
									count++;
									attackSqures.push_back(temp);
								}
							}
							else if (!pieces[temp.x][temp.y]->IsWhite() && i < 2) {
								if (!checkForPins) {
									count++;
									attackSqures.push_back(temp);
								}
							}
						break;
					case 'K':
						if (dis == 1) {
							if (!checkForPins) {
								count++;
								attackSqures.push_back(temp);
							}
						}

					} // end switch		
					if (check == checkForPins && !pinFound)
						break;
				}
				else
				{
					if (checkForPins)
						break;
					checkForPins = 1;
				}
					
				if (pinFound)
					break;
			}
			temp.x += dirX;
			temp.y += dirY;
			dis++;
		}// end while
		temp = location;
	}// end for
	// check for Knights :
	std::vector<sf::Vector2i> possibleMoves = { {location.x + 2, location.y + 1}, {location.x + 2, location.y - 1 },{location.x - 2, location.y + 1},
		{location.x - 2, location.y - 1}, {location.x + 1, location.y + 2}, {location.x - 1, location.y + 2},
	{location.x + 1, location.y - 2}, {location.x - 1, location.y - 2} };
	for (auto move : possibleMoves)
		if (InBoard(move) && pieces[move.x][move.y] && pieces[move.x][move.y]->GetType()[1] == 'N' && pieces[move.x][move.y]->IsWhite() != isWhite) {
			count++;
			attackSqures.push_back(move);
		}
	return count;
	
	//int dirX, dirY;
	//int directions[][8] = { {-1,-1}, {-1, 1}, {1, -1}, {1, 1}, {0, -1}, {0, 1}, {1, 0}, {-1, 0} };
	//sf::Vector2i temp = location;
	//for (int i = 0; i < 8; i++)
	//{
	//	dirX = directions[i][0];
	//	dirY = directions[i][1];
	//	temp.x += dirX;
	//	temp.y += dirY;
	//	int dis = 1;
	//	while (InBoard(temp))
	//	{
	//		if (pieces[temp.x][temp.y])
	//		{
	//			if (pieces[temp.x][temp.y]->IsWhite() != isWhite)
	//			{
	//				switch (pieces[temp.x][temp.y]->GetType()[1])
	//				{
	//				case 'Q':
	//					return 1;
	//				case 'B':
	//					if (i < 4)
	//						return 1;
	//					break;
	//				case 'R':
	//					if (i >= 4)
	//						return 1;
	//					break;
	//				case 'p':
	//					if (dis == 1)
	//						if (pieces[temp.x][temp.y]->IsWhite() && i >= 2 && i <= 3)
	//							return 1;
	//						else if (!pieces[temp.x][temp.y]->IsWhite() && i < 2)
	//							return 1;
	//					break;
	//				case 'K':
	//					if (dis == 1)
	//						return 1;
	//					break;
	//				} // end switch

	//			}
	//			break;
	//		}
	//		temp.x += dirX;
	//		temp.y += dirY;
	//		dis++;
	//	}// end while
	//	temp = location;
	//}
	//// check for Knights :
	//std::vector<sf::Vector2i> possibleMoves = { {location.x + 2, location.y + 1}, {location.x + 2, location.y - 1 },{location.x - 2, location.y + 1},
	//	{location.x - 2, location.y - 1}, {location.x + 1, location.y + 2}, {location.x - 1, location.y + 2},
	//{location.x + 1, location.y - 2}, {location.x - 1, location.y - 2} };
	//for (auto move : possibleMoves)
	//	if (InBoard(move) && pieces[move.x][move.y] && pieces[move.x][move.y]->GetType()[1] == 'N' && pieces[move.x][move.y]->IsWhite() != isWhite)
	//		return 1;
	//return 0;





}
int Piece::IsEnPassent(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS])
{

	if (isWhite)
	{
		if (this->indexes.x != 3)
			return 0;
		if (lastMove[0] == sf::Vector2i(dest.x - 1, dest.y) && lastMove[1] == sf::Vector2i(dest.x + 1, dest.y) && pieces[dest.x + 1][dest.y]->GetType() == "bp")
		{
			return 1;
		}
	}
	else
	{
		if (this->indexes.x != 4)
			return 0;
		if (lastMove[0] == sf::Vector2i(dest.x + 1, dest.y) && lastMove[1] == sf::Vector2i(dest.x - 1, dest.y) && pieces[dest.x - 1][dest.y]->GetType() == "wp")
		{
			return 1;
		}
	}
	return 0;
}
int Piece::IsMovePinned(sf::Vector2i move, sf::Vector2i kingIndexes, Piece* pieces[DIMENSIONS][DIMENSIONS])
{
	if (GetType()[1] == 'p' && IsEnPassent(move, pieces))
	{
		auto attacker = pieces[this->indexes.x][this->indexes.y];
		auto defender = pieces[this->indexes.x][move.y];
		pieces[this->indexes.x][this->indexes.y] = nullptr;
		pieces[this->indexes.x][move.y] = nullptr;
		if (squreUnderAttack(pieces, kingIndexes))
		{
			pieces[this->indexes.x][this->indexes.y] = attacker;
			pieces[this->indexes.x][move.y] = defender;
			return true;
		}
		pieces[this->indexes.x][this->indexes.y] = attacker;
		pieces[this->indexes.x][move.y] = defender;
	}
	for (int i = 0; i < pinSqures.size(); i++)
	{
		std::vector<sf::Vector2i> squresBetween = GetSquresBetween(kingIndexes, pinSqures[i]);
		if (std::find(squresBetween.begin(), squresBetween.end(), this->indexes) != squresBetween.end()) {
			if (move != pinSqures[i] && std::find(squresBetween.begin(), squresBetween.end(), move) == squresBetween.end())
				return true;
		}
			
	}
	return false;
}

std::vector<sf::Vector2i> Piece::GetSquresBetween(sf::Vector2i a, sf::Vector2i b)
{
	std::vector<sf::Vector2i> squresBetween = std::vector<sf::Vector2i>();
	int dirX, dirY;
	dirX = a.x - b.x;
	dirY = a.y - b.y;
	if (dirX) dirX /= abs(dirX);
	if (dirY) dirY /= abs(dirY);
	sf::Vector2i temp = b;
	temp.x += dirX;
	temp.y += dirY;
	while (temp != a)
	{
		squresBetween.push_back(temp);
		temp.x += dirX;
		temp.y += dirY;
	}
	return squresBetween;
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







std::vector<sf::Vector2i> Piece::GetValidMoves(Piece* pieces[DIMENSIONS][DIMENSIONS], std::vector<sf::Vector2i> kings)
{
	sf::Vector2i kingIndexes;
	if (isWhite)
		kingIndexes = kings.front();
	else 
		kingIndexes = kings.back();
	std::vector<sf::Vector2i> possibleMoves = GetPossibleMoves(pieces);	
	if (kingIndexes == this->indexes)
	{
		// if the piece is the king than return from all the possible squres to move, the squres that not under attack.
		Piece* kingPiece = pieces[kingIndexes.x][kingIndexes.y];
		pieces[kingIndexes.x][kingIndexes.y] = nullptr;
		for (int i = possibleMoves.size() - 1; i >= 0; i--)
		{
			if(squreUnderAttack(pieces, possibleMoves[i]))
				possibleMoves.erase(possibleMoves.begin() + i);
		}
		pieces[kingIndexes.x][kingIndexes.y] = kingPiece;
		return possibleMoves;
	}
	int count = squreUnderAttack(pieces, kingIndexes);
	// from now we check for other pieces than the king:
	if (count == 2)// in a case of double check only the king can move.
	{
		pinSqures.clear();
		attackSqures.clear();
		return std::vector<sf::Vector2i>();
	}
	else
	{
		if (count == 1)
		{
			// if the king is in check and the piece is not the king, the piece can only move in order to:
			// 1. block the check
			// 2. capture the attacking 
			// the piece can only move if it is not a pin.
			sf::Vector2i squre = attackSqures.back();
			attackSqures.pop_back();
			
			if (pieces[squre.x][squre.y]->GetType()[1] == 'N')
			{
				// if the attacking piece is knight than the piece can only move to capture the attacking piece (only if the piece is not pinned).
				for (int i = possibleMoves.size() - 1; i >= 0; i--)
				{
					if (possibleMoves[i] != squre || IsMovePinned(possibleMoves[i], kingIndexes, pieces)) // check if the possible move is not a capture.
						possibleMoves.erase(possibleMoves.begin() + i);
				}
				
			}
			else
			{
				std::vector<sf::Vector2i> squresBetween = GetSquresBetween(kingIndexes, squre);
				for (int i = possibleMoves.size() - 1; i >= 0; i--)
				{
					if (!IsMovePinned(possibleMoves[i], kingIndexes, pieces) && possibleMoves[i] == squre) // check if the possible move is capture.
						continue;
					if (std::find(squresBetween.begin(), squresBetween.end(), possibleMoves[i]) == squresBetween.end())
						possibleMoves.erase(possibleMoves.begin() + i);
				}
				
			}
			
			pinSqures.clear();
			return possibleMoves;
		}
		else
		{
			// if the king is not in check than the move is valid unless it pinned by an enemy piece..
			for (int j = possibleMoves.size() - 1; j >= 0; j--)
			{
				if(IsMovePinned(possibleMoves[j], kingIndexes, pieces))
					possibleMoves.erase(possibleMoves.begin() + j);
			}
			return possibleMoves;
		}
	}
		
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






