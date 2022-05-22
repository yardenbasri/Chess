#pragma once

class TranspositionTable
{
public:
	TranspositionTable();
	TranspositionTable(__int64 hash_key, int depth,int flag,int score);
	// get
	/*__int64* Read();
	__int64 GetHashKey();
	int GetDepth();
	int GetFlag();
	int GetScore();*/
	// set 
	//void Write(__int64 hash_key, int depth, int flag, int score);


	unsigned __int64 hash_key;
	int depth;
	int flag;
	int score;
};

