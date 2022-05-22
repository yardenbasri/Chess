#include "TranspositionTable.h"

TranspositionTable::TranspositionTable()
{
}

TranspositionTable::TranspositionTable(__int64 hash_key, int depth, int flag, int score)
{
	this->hash_key = hash_key;
	this->depth = depth;
	this->flag = flag;
	this->score = score;
}
