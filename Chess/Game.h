#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include "Constants.h"
#include "Piece.h"
#include "Button.h"
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
	void Move(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter); // change the pointer of the piece that need to move and the sprite`s position.
	void UndoMove(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter); // The function undo a move.
	void UpdatesAfterMove();
	void MarkSqure(sf::Vector2i indexes, sf::Color color); // gets the indexes of a certian squre and a color and mark it.
	void cleanSqure(sf::Vector2i indexes); // gets the indexes of a marked squre and cleans it.
	void CleanBoard(); // restart the board
	/* the function gets a vector of all possible moves indexes and return a vector of circleShapes in the current indexes.*/
	std::vector<sf::CircleShape> GetPossibleCircles(std::vector<sf::Vector2i> possibleMoves); 
	/* the function gets the type of castling and the indexes of the king and rock and does the castle.*/
	// move functions : 
	void HandleClickes(sf::Vector2i indexes); // gets the indexes of the mouse position when it was clicked and handles all the clickes.
	void RegularMove(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter);
	void Castling(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter);
	void Promotion(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter, bool isWhite);
	void EnPassent(bool isWhite, sf::Vector2i indexesBefore, sf::Vector2i indexesAfter);
	/*the function checks if the move was castling and if is was than it checks if the castling is a legal move 
	if it is than return 1 else 0. if the move is not castling than return 1.*/
	int IsCastlingLegal(sf::Vector2i indexesBefore, sf::Vector2i indexesAfter);
	std::vector<sf::Vector2i> GetLegalMoves(Piece *piece);
	int InBoard(sf::Vector2i indexes);
	/* A function that return a vector of the indexes of all the legal moves. the steps are:
	1. generate all possible moves.
	2. for each move, make the move.
	3. generate all opponent possible moves.
	4. for each of the opponent`s moves, see if they attack your king.
	5. if they do attack your king, it is`nt a valid move.*/
	
	int InCheck();
	int squreUnderAttack(sf::Vector2i location);
	std::vector<sf::Vector2i> GetKingsLocation(); // the function returns two vector2i variables first for wk pos and second for bk pos.
	void HandleChecks(); // the function check if the kings in check and mark them in red if they are.
	void checkmateOrStalemate(); // the function check if a checkmate or stalemate happend.
	void RotatePieces(); // rotates all the pieces in the board.
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> AllLegalMoves(bool isWhite);
	void UpdateEvents(); // this function checks for current events and act accordingly.
	void UpdateBoard();
	void DrawPromotionOptions();
	void HandlePromotionOptions(sf::Vector2i indexes);
	std::string TranslateMove(std::pair<sf::Vector2i, sf::Vector2i> move);
	// virtual functions
	void virtual Print() = 0; // this function draws to the screen all the sprites.
	void virtual Update() = 0; // this function updates the game situation.
	void virtual Init() = 0; // game loop.
	bool virtual PlayerTurn(sf::Vector2i indexes) = 0;
protected:
	// window
	sf::RenderWindow* window; 
	// textures
	sf::Texture boardTex;
	std::map<std::string, sf::Texture> piecesTex;
	// fonts
	sf::Font arial_font;
	// texts
	sf::Text text_board;
	sf::Text text_msg;
	// sprites
	sf::Sprite board;
	std::vector<sf::RectangleShape> squres;
	std::vector<sf::Vector2i> legalMoves;
	// squres
	Piece* pieces[DIMENSIONS][DIMENSIONS]; // A pointer's matrix that represent the squres.
	// colors
	sf::Color darkSqure; 
	sf::Color lightSqure; 
	sf::Color markedSqure; 
	int colorIndex = 0;
	bool keyReleased = true;
	std::vector<sf::Color> lightSqures = { sf::Color(165, 42, 42), sf::Color::Cyan, sf::Color::Magenta, sf::Color(232, 235, 239) };
	std::vector<sf::Color> darkSqures  = { sf::Color(241, 217, 192), sf::Color::Blue, sf::Color::Green, sf::Color(125, 135, 150) };

	// bool variables
	bool mReleased; // A bool that represent if mouse released.
	int whiteTurn; // 1 if it is white move , 0 if it is black move.
	int checkmate;
	int stalemate;
	int gameOver;
	bool inCheck;
	bool isUndoPossible;
	bool gamePaused;
	// game vars
	sf::Vector2i sqSelected; // save the indexes of the last selected squre.
	std::vector<sf::Vector2i> markedSqures; // A vector that represents all the squres that need to be marked.
	std::vector<Button*> buttons;
	//std::vector<sf::Vector2i> lastMove; // the last move that has been played.
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> lastMoveVec;
	std::vector<bool> pieceMovedVec;
	enum moveTypes { REGULARMOVE, PROMOTION, WHITE_ENPASSENT, BLACK_ENPASSENT, KING_CASTLING, QUEEN_CASTLING };
	enum promotionTypes {QUEEN, KNIGHT, ROCK, BISHOP};
	// promotion :
	std::vector<int> promotionTypeVec;
	int promotionChoice;
	std::vector<sf::Vector2i> promotionIndexes;
	std::vector<Piece*> promotionPieces;
	std::vector<Piece*> promotionPiecesReplace;
	int isPromInit = 1;
	//
	std::vector<int> moveTypeVec;
	std::string lastType = "--";
	std::vector<std::string> lastTypeVec;
	std::vector<Piece*> capturedPieces;
	Piece* capturedSqure = nullptr;
	int flag;
	std::vector<sf::Vector2i> playerClickes; // a vector that saves the player indexes of the squres he has clicked.
	std::vector<sf::CircleShape> possibleCircles; // A vector of all the circles shapes that need to be draw.
	//position 1
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
	// position 5
	/*std::string boardStatus[DIMENSIONS][DIMENSIONS] =
	{
		{"bR", "bN", "bB", "bQ", "--", "bK", "--", "bR"},
		{"bp", "bp", "--", "wp", "bB", "bp", "bp", "bp"},
		{"--", "--", "bp", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "wB", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"}, 
		{"wp", "wp", "wp", "--", "wN", "bN", "wp", "wp"},
		{"wR", "wN", "wB", "wQ", "wK", "--", "--", "wR"}
	};*/
	// position 3
	/*std::string boardStatus[DIMENSIONS][DIMENSIONS] =
	{
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "bp", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "bp", "--", "--", "--", "--"},
		{"wK", "wp", "--", "--", "--", "--", "--", "bR"},
		{"--", "wR", "--", "--", "--", "bp", "--", "bK"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "wp", "--", "wp", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"}
	};*/
	//// position 4
	/*std::string boardStatus[DIMENSIONS][DIMENSIONS] =
	{
		{"bR", "--", "--", "--", "bK", "--", "--", "bR"},
		{"wp", "bp", "bp", "bp", "--", "bp", "bp", "bp"},
		{"--", "bB", "--", "--", "--", "bN", "bB", "wN"},
		{"bN", "wp", "--", "--", "--", "--", "--", "--"},
		{"wB", "wB", "wp", "--", "wp", "--", "--", "--"},
		{"bQ", "--", "--", "--", "--", "wN", "--", "--"},
		{"wp", "bp", "--", "wp", "--", "--", "wp", "wp"},
		{"wR", "--", "--", "wQ", "--", "wR", "wK", "--"}
	};*/
	// king vs king & rock
	/*std::string boardStatus[DIMENSIONS][DIMENSIONS] =
	{
		{"--", "--", "--", "--", "--", "--", "--", "bR"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "wK", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "--", "--", "--", "--", "--", "--", "--"},
		{"--", "bK", "--", "--", "--", "--", "--", "--"}
	};*/
	

	/*
	What do i have to do next :
	- whenever someone marks a squre -> outputs all possible moves. # done
	- whenever check happens mark the king in red (like lichess). # done
	- make another function of all legal moves including check. # done
	- end the game when checkmate happens. # done
	- en passent. # done
	- pown promotion. # done only to queen.
	- castling. # done 
	- use sfml-> views for screen manipulating. # done
	- make a ui before the game and after the game ends.
	- make a piece moved animation.
	- make voice each turn and mark the last squre that a piece was moved to.
	- make voice of checkmate.
	- make the code better.
	- make option of choosing black / white / randomly.
	- make time options.
	- make ai minimax alpha beta.
	- make online (sfml / boost -> asio).
	- when mouse hover over possible moves -> light the squre.
	- make a small data base.
	- A final menu where u have an option between sign up or sign in, 
		than play offline / online / against the cp, than choose time limit, than black / white / random.
	*/
	
};

