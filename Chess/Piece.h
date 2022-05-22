#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "Constants.h"



class Piece 
{
public:
	// constructor
	Piece(sf::Texture& pieceTex, sf::Vector2f position, 
		int isWhite, sf::Vector2i indexes, std::string type); // Ctor
	Piece(); // defualt Ctor
	//piece functions
	void Print(sf::RenderWindow* window); // gets the window pointer and prints the piece.
	// get functions
	sf::Sprite GetSprite();
	sf::Vector2f GetPosition();
	sf::Vector2i GetIndexes();
	std::string GetType();
	int IsWhite();
	// set functions
	void SetPosition(sf::Vector2f position);
	void SetIndexes(sf::Vector2i indexes);
	void SetSpriteColor(sf::Color color);
	void SetLastMove(std::pair<sf::Vector2i, sf::Vector2i> lastMove);
	void setPieceMoved(bool pieceMoved);
	// piece functions : 
	/* the function gets source click and dest click and the pieces.
	the function returns 1 if the dest click has no piece or have a piece with an opposite color.
	else it returns 0.
	*/
	int OppositeColors(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS]);
	int InRange(sf::Vector2i move);// get a move and returns 1 if the move in the range of the board. else 0.
	int PieceMoved();
	void RotatePiece(float rot); 
	int InBoard(sf::Vector2i indexes);
	void UpdatePossibleMoves(std::vector<sf::Vector2i> *possibleMoves, int xDir, int yDir, Piece* pieces[DIMENSIONS][DIMENSIONS], int limit);
	int squreUnderAttack(Piece* pieces[DIMENSIONS][DIMENSIONS], sf::Vector2i location);
	int IsEnPassent(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS]);
	int IsMovePinned(sf::Vector2i move, sf::Vector2i kingIndexes, Piece* pieces[DIMENSIONS][DIMENSIONS]);
	std::vector<sf::Vector2i> GetSquresBetween(sf::Vector2i a, sf::Vector2i b); // gets to squres and returns all the squres between them.
	// virtual functions
	virtual int IsPossibleMove(sf::Vector2i dest, Piece* pieces[DIMENSIONS][DIMENSIONS]) = 0;
	/* A virtual function.
	 Each class that inherits piece implemnt it differently.
	 The function gets the dest move and checks if the move is legal.
	 returns 1 or 0.*/
	virtual std::vector<sf::Vector2i> GetPossibleMoves(Piece* pieces[DIMENSIONS][DIMENSIONS]) ;
	/* A virtual function.
	 The function gets the array of all the pieces and returns all the possible moves.
	 This function uses IsLegalMove.
	 The function is virtual becuse pown has a different implementation than the other pieces.
	 returns a vector off all the possible moves.*/

	std::vector<sf::Vector2i> GetValidMoves(Piece* pieces[DIMENSIONS][DIMENSIONS], std::vector<sf::Vector2i> kings);

	


	virtual std::vector<sf::Vector2i> GetCastlingMoves(Piece* pieces[DIMENSIONS][DIMENSIONS]); 
	/* This virtual function will get possible moves for castling.
	Only king will implement this function.*/
protected:
	sf::Sprite piece; // sprite of the piece.
	sf::Vector2f position; // current position.
	int isWhite; // 1 if white, 0 if black.
	sf::Vector2i indexes; // the indexes in the board of the current position.
	sf::Vector2i first_indexes; // save the first position of the piece.
	/*the type of a piece which is represented by two letters :
	1. for color.
	2. for type of piece.
	example: black Queen -> bQ.*/
	std::string type; 
	int pieceMoved;
	sf::Vector2i lastMove[2];

private:
	std::vector<sf::Vector2i> attackSqures;
	std::vector<sf::Vector2i> pinSqures;
	
};

