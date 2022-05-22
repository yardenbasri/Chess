#pragma once
#include "Game.h"
#include "TranspositionTable.h"
#include <Windows.h>
#include <Wincrypt.h>
#include <inttypes.h>
#include <random>
#include <ctime>
#include <chrono>
#include <stdint.h>

#define no_hash_entry 100000


class AiGame :
    public Game
{
public:
    // basic functions:
    AiGame();
    void Init(); // loop function.
    void Update();
    void Print(); // print all needed sprites to the screen.
    bool PlayerTurn(sf::Vector2i indexes);
    // evaluations:
    int evaluate(); // evaluate the board.
    int EndGameEval(sf::Vector2i friendlyKingSqure, sf::Vector2i opponentKingSqure, float endGameWeight);
    float CalcEndGameWeight(int materialCountWithoutPawns, int endGameMaterialStart);
    int CountMaterial(bool isWhite);
    int CountPieceType(std::string type);
    int IsEndGame();
    // minimax
    int minimax(int depth, int isMax);
    int minimax(int depth, int isMax, int alpha, int beta);
    // best move
    std::pair<sf::Vector2i, sf::Vector2i> findBestMove();
    void SmartAi();
    // analisys
    int MoveGenerarionTest(int depth, int isWhite, int startDepth);
    // A search function
    int Search(int depth, bool isWhite);
    int NegaMax(int depth, bool isWhite,int alpha, int beta);
    // order moves for pure alpha beta purning function
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> OrderMoves(std::vector<std::pair<sf::Vector2i, 
        sf::Vector2i>> moves);
    // text msg
    sf::Text eval_msg;
    sf::Text aiMove_msg;
    sf::Text depth_msg;
   // A map of the pieces scores

    std::map<char, int> scores =
    {   {'p', 100},
        {'N', 300},
        {'B', 320},
        {'R', 500},
        {'Q', 900}
    };

    std::vector<std::vector<int>> FlipTableScores(std::vector<std::vector<int>> tableScores);


#pragma region TranspositionTable
    // pseudo random number state
    unsigned long long int state = 1804289383;
    // pseudo random number generator
    unsigned long Random32bit();
    unsigned __int64 Random64bit();
    unsigned __int64 pieces_keys[NUM_OF_PIECES][DIMENSIONS * DIMENSIONS];
    std::map<int, std::string> piece_key_value;
    unsigned __int64 side_key;
    void init_zorbist();
    unsigned __int64 ZorbistKey();
    void ClearTranspositionTable();
    void WriteHashEntry(int score, int depth, int hash_flag);
    unsigned __int64 ReadHashEntry(int alpha, int beta, int depth);
    std::mt19937 mt = std::mt19937(01234567);
#pragma endregion

   
  
    
private:
    std::pair<sf::Vector2i, sf::Vector2i> nextMove;
    std::map<std::string, int> moveCounter;
    int a = 1;
#pragma region PieceSqureTables
    /* std::vector<std::vector<int>> knightScores =
   {
       {100, 100, 100, 100, 100, 100, 100, 100},
       {100, 200, 200, 200, 200, 200, 200, 100},
       {100, 200, 300, 300, 300, 300, 200, 100},
       {100, 200, 300, 400, 400, 300, 200, 100},
       {100, 200, 300, 400, 400, 300, 200, 100},
       {100, 200, 300, 300, 300, 300, 200, 100},
       {100, 200, 200, 200, 200, 200, 200, 100},
       {100, 100, 100, 100, 100, 100, 100, 100},

   };
   std::vector<std::vector<int>> queenScores =
   {
       {100, 100, 100, 300, 100, 100, 100, 100},
       {100, 200, 300, 300, 300, 100, 100, 100},
       {100, 400, 300, 300, 300, 400, 200, 100},
       {100, 200, 300, 300, 300, 200, 200, 100},
       {100, 200, 300, 300, 300, 200, 200, 100},
       {100, 400, 300, 300, 300, 400, 200, 100},
       {100, 100, 200, 300, 300, 100, 100, 100},
       {100, 100, 100, 300, 100, 100, 100, 100},

   };
   std::vector<std::vector<int>> bishopScores =
   {
       {400, 300, 200, 100, 100, 200, 300, 400},
       {300, 400, 300, 200, 200, 300, 400, 300},
       {200, 300, 400, 300, 300, 400, 300, 200},
       {100, 200, 300, 400, 400, 300, 200, 100},
       {100, 200, 300, 400, 400, 300, 200, 100},
       {200, 300, 400, 300, 300, 400, 300, 200},
       {300, 400, 300, 200, 200, 300, 400, 300},
       {400, 300, 100, 100, 100, 200, 300, 400},

   };
   std::vector<std::vector<int>> rookScores =
   {
       {400, 300, 400, 400, 400, 400, 300, 400},
       {400, 400, 400, 400, 400, 400, 400, 400},
       {100, 100, 200, 300, 300, 200, 100, 100},
       {100, 200, 300, 400, 400, 300, 200, 100},
       {100, 200, 300, 400, 400, 300, 200, 100},
       {100, 100, 200, 200, 200, 200, 100, 100},
       {400, 400, 400, 400, 400, 400, 400, 400},
       {400, 300, 400, 400, 400, 400, 300, 400},

   };
   std::vector<std::vector<int>> bPownScores =
   {
       {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
       {100, 100, 100, 0  , 0  , 100, 100, 100},
       {100, 100, 200, 300, 300, 200, 100, 100},
       {100, 140, 300, 400, 400, 300, 140, 100},
       {200, 300, 300, 500, 500, 300, 300, 200},
       {500, 600, 600, 700, 700, 600, 600, 500},
       {800, 800, 800, 800, 800, 800, 800, 800},
       {800, 800, 800, 800, 800, 800, 800, 800},

   };
   std::vector<std::vector<int>> wPownScores =
   {
        {800, 800, 800, 800, 800, 800, 800, 800},
        {800, 800, 800, 800, 800, 800, 800, 800},
        {500, 600, 600, 700, 700, 600, 600, 500},
        {200, 300, 300, 500, 500, 300, 300, 200},
        {100, 200, 300, 400, 400, 300, 200, 100},
        {100, 100, 200, 300, 300, 200, 100, 100},
        {100, 100, 100, 0  , 0, 100, 100, 100},
        {0  , 0  , 0  , 0  , 0, 0, 0, 0},

   };
   std::vector<std::vector<int>> KingScores =
   {
       {000, 400, 000, 000, 000, 400, 000, 000},
       {000, 000, 000, 000, 000, 000, 000, 000},
       {000, 000, 000, 000, 000, 000, 000, 000},
       {000, 000, 000, 000, 000, 000, 200, 100},
       {000, 000, 000, 000, 000, 000, 000, 000},
       {000, 000, 000, 000, 000, 000, 000, 000},
       {000, 000, 000, 000, 000, 000, 000, 000},
       {000, 000, 000, 000, 000, 000, 000, 000},

   };*/
    std::vector<std::vector<int>> knightScores =
    {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
    };
    std::vector<std::vector<int>> queenScores =
    {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        {-5,  0,  5,  5,  5,  5,  0, -5},
        {0,  0,  5,  5,  5,  5,  0, -5},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
    };
    std::vector<std::vector<int>> bishopScores =
    {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
    };
    std::vector<std::vector<int>> rookScores =
    {
        {0,  0,  0,  0,  0,  0,  0,  0},
        {5, 10, 10, 10, 10, 10, 10,  5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {0,  0,  0,  5,  5,  0,  0,  0}
    };
    std::vector<std::vector<int>> pawnScores =
    {
        {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {5,  5, 10, 25, 25, 10,  5,  5},
        {0,  0,  0, 20, 20,  0,  0,  0},
        {5, -5,-10,  0,  0,-10, -5,  5},
        {5, 10, 10,-20,-20, 10, 10,  5},
        {0,  0,  0,  0,  0,  0,  0,  0},

    };
    std::vector<std::vector<int>> kingMiddleScores =
    {
         {-30,-40,-40,-50,-50,-40,-40,-30},
         {-30,-40,-40,-50,-50,-40,-40,-30},
         {-30,-40,-40,-50,-50,-40,-40,-30},
         {-30,-40,-40,-50,-50,-40,-40,-30},
         {-20,-30,-30,-40,-40,-30,-30,-20},
         {-10,-20,-20,-20,-20,-20,-20,-10},
         {20, 20,  0,  0,  0,  0, 20, 20},
         {20, 30, 10,  0,  0, 10, 30, 20},

    };
    std::vector<std::vector<int>> KingEndScores =
    {
        {-50,-40,-30,-20,-20,-30,-40,-50},
        {-30,-20,-10,  0,  0,-10,-20,-30},
        {-30,-10, 20, 30, 30, 20,-10,-30},
        {-30,-10, 30, 40, 40, 30,-10,-30},
        {-30,-10, 30, 40, 40, 30,-10,-30},
        {-30,-10, 20, 30, 30, 20,-10,-30},
        {-30,-30,  0,  0,  0,  0,-30,-30},
        {-50,-30,-30,-30,-30,-30,-30,-50},

    };


    std::map<std::string, std::vector<std::vector<int>>> piecePositionScores =
    {
        {"wN", knightScores},
        {"wQ", queenScores},
        {"wB", bishopScores},
        {"wR", rookScores},
        {"wp", pawnScores},
        {"wKM", kingMiddleScores},
        {"wKE", KingEndScores},
        {"bN", FlipTableScores(knightScores)},
        {"bQ", FlipTableScores(queenScores)},
        {"bB", FlipTableScores(bishopScores)},
        {"bR", FlipTableScores(rookScores)},
        {"bp", FlipTableScores(pawnScores)},
        {"bKM", FlipTableScores(kingMiddleScores)},
        {"bKE", FlipTableScores(KingEndScores)}
    };
#pragma endregion
   /* struct TTtable {
        unsigned __int64 hash_key;
        int depth;
        int flag;
        int score;
    }tablePtr;
    TTtable ttTable[hash_size];*/
    TranspositionTable ttTable[hash_size];
};

