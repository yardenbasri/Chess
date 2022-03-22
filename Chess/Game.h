#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include "Constants.h"
#include "Piece.h"
#include <vector>
// include all pieces:
#include "Pown.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Rock.h"
#include "Knight.h"

class Game
{
public:
	Game();
	// base functions
	void Start(); // start calls the functions that need to be done only once and than call to Init.
	void LoadData(); // load all the files to there textures.
	void LoadSprites(); // load the textures to there sprites.
	void CreateBoard(); // init the recatngle vector correctly with 8*8 squres.
	void DrawNumbersLetters(sf::RenderWindow* window);
	// game functions
	sf::Vector2f Indexes2Position(sf::Vector2i indexes); // gets two indexes and return the squre position.
	sf::Vector2i Position2Indexes(sf::Vector2f mPos); // gets the position and return the indexes.
	void Move(); // change the pointer of the piece that need to move and the sprite`s position.
	void MarkSqure(sf::Vector2i indexes); // gets the indexes of a certian squre and mark it.
	void cleanSqure(sf::Vector2i indexes); // gets the indexes of a marked squre and cleans it.
	double calculateDistance(sf::Vector2f a, sf::Vector2f b); // calculate the distance between two vectors.
	void CleanBoard(); // restart the board
	/* the function gets a vector of all possible moves indexes and return a vector of circleShapes in the current indexes.*/
	std::vector<sf::CircleShape> GetPossibleCircles(std::vector<sf::Vector2i> possibleMoves); 
	/* the function gets the type of castling and the indexes of the king and rock and does the castle.*/
	void Castling(sf::Vector2i king, sf::Vector2i rock, int rightCastling);
	/*the function checks if the move was castling and if is was than it checks if the castling is a legal move 
	if it is than return 1 else 0. if the move is not castling than return 1.*/
	int IsCastlingLegal(); 
	std::vector<sf::Vector2i> GetLegalMoves(Piece *piece);
	/* A function that return a vector of the indexes of all the legal moves. the steps are:
	1. generate all possible moves.
	2. for each move, make the move.
	3. generate all opponent possible moves.
	4. for each of the opponent`s moves, see if they attack your king.
	5. if they do attack your king, it is`nt a valid move.*/
	// virtual functions
	int InCheck();
	int squreUnderAttack(sf::Vector2i location);
	std::vector<sf::Vector2i> GetKingsLocation(); // the function returns two vector2i variables first for wk pos and second for bk pos.
	int IsLegalMove(std::vector<sf::Vector2i> moves, sf::Vector2i move); // the function gets all the legal moves and a move and check if the move in that list.
	void HandleChecks(); // the function check if the kings in check and mark them in red if they are.
	void checkmateOrStalemate(); // the function check if a checkmate or stalemate happend.
	virtual void UpdateEvents() = 0; // this function checks for current events and act accordingly.
	void virtual Print() = 0; // this function draws to the screen all the sprites.
	void virtual Update() = 0; // this function updates the game situation.
	void virtual Init() = 0; // game loop.
protected:
	// window
	sf::RenderWindow* window; 
	// textures
	sf::Texture boardTex;
	std::map<std::string, sf::Texture> piecesTex;
	// fonts
	sf::Font font;
	// texts
	sf::Text text_board;
	sf::Text text_msg;
	// sprites
	sf::Sprite board;
	std::vector<sf::RectangleShape> squres;
	// squres
	Piece* pieces[DIMENSIONS][DIMENSIONS]; // A pointer's matrix that represent the squres.
	// integers variables
	int mReleased; // A bool that represent if mouse released.
	int whiteTurn; // 1 if it is white move , 0 if it is black move.
	int checkmate;
	int stalemate;
	int gameOver;
	// game vars
	//std::vector<sf::Vector2i> lastMove; // the last move that has been played.
	sf::Vector2i lastMove[2];
	int flag;
	std::vector<sf::Vector2i> playerClickes; // a vector that saves the player indexes of the squres he has clicked.
	std::string boardStatus[DIMENSIONS][DIMENSIONS] =
	{
		{"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
		{"bp", "bp", "bp", "bp", "bp", "bp", "bp", "bp"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"wp", "wp", "wp", "wp", "wp", "wp", "wp", "wp"},
		{"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}
	};
	std::vector<sf::CircleShape> possibleCircles; // A vector of all the circles shapes that need to be draw.

	/*
	What do i have to do next :
	- whenever someone marks a squre -> outputs all possible moves. # done
	- whenever check happens mark the king in red (like lichess). # done
	- make another function of all legal moves including check. # done
	- end the game when checkmate happens. # done
	- en passent. # done
	- pown promotion. # done only to queen.
	- castling. # done 
	- when mouse hover over possible moves -> light the squre.
	- make a piece moved animation.
	- make voice each turn.
	- make voice of checkmate.
	- make the code better.
	- use sfml-> views for screen manipulating.
	- make a ui before the game and after the game ends.
	- make a small data base.
	- make option of choosing black / white / randomly.
	- make time options.
	- make ai minimax alpha beta.
	- make online (sfml / boost -> asio).
	- A final menu where u have an option between sign up or sign in, 
		than play offline / online / against the cp, than choose time limit, than black / white / random.
	*/
	
};

