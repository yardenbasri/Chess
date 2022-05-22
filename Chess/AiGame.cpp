#include "AiGame.h"

AiGame::AiGame()
{
	srand(time(NULL));
	sqSelected = { -1,-1 };
	whiteTurn = 1;
	isUndoPossible = false;
	mReleased = true;
	checkmate = 0;
	stalemate = 0;
	gameOver = 0;
	promotionChoice = 0;
	promotionTypeVec.push_back(QUEEN);
	
	
	
}

void AiGame::Init()
{
	init_zorbist();
	eval_msg.setFont(Game::arial_font);
	aiMove_msg.setFont(arial_font);
	depth_msg.setFont(arial_font);
	eval_msg.setPosition(DIMENSIONS / 2 - 5, -100);
	aiMove_msg.setPosition(DIMENSIONS / 2 - 5, -80);
	depth_msg.setPosition(DIMENSIONS / 2 - 5, 0 + -60);
	while (window->isOpen())
	{
		UpdateEvents();
		Update();
		Print();
	}
}


void AiGame::Update()
{
	if (checkmate) {
		gameOver = 1;
		if (whiteTurn)
			text_msg.setString("Black won !");
		else
			text_msg.setString("White won !");
	}
	if (stalemate)
	{
		gameOver = 1;
		text_msg.setString("Stalemate !");
	}
	if (gamePaused)
	{
		for (auto btn : buttons)
			btn->HandleMouseHover(*window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (buttons[0]->IsMouseHover(*window)) {
				CleanBoard();
				gamePaused = false;
			}
			else if (buttons[1]->IsMouseHover(*window)) {
				gamePaused = false;
			}
			else if (buttons[2]->IsMouseHover(*window)) {
				window->close();
			}

		}

	}
	if (!gameOver && !gamePaused)
	{	
		if(whiteTurn)
			UpdateBoard();
		else
		{
			if (!promotionChoice) {
				auto start = std::chrono::steady_clock::now();
				SmartAi();
				auto end = std::chrono::steady_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;
				std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
				whiteTurn = !whiteTurn;
				HandleChecks();
				checkmateOrStalemate();
			}
			UpdateBoard();
		}
	}
	else
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttons[0]->IsMouseHover(*window))
			CleanBoard();
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		CleanBoard(); // restart the game.
	}

}

void AiGame::Print()
{
	/*sf::View view = sf::View(sf::FloatRect(0, 0, SCREEN_SIZE, SCREEN_SIZE));
	view.setViewport(sf::FloatRect(0.1f, 0.2f, 0.8f, 0.8f));
	window->setView(view);*/
	window->clear();

	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			window->draw(squres[i * DIMENSIONS + j]);
		}
	}

	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j])
			{
				pieces[i][j]->Print(window);
			}
		}
	}
	DrawNumbersLetters(window);
	for (auto circle : possibleCircles) {
		window->draw(circle);
	}
	
	if (gamePaused)
		for (auto btn : buttons)
			btn->DrawTo(*window);
	if (gameOver)
	{
		window->draw(text_msg);
		buttons[0]->DrawTo(*window);
		buttons[0]->HandleMouseHover(*window);
	}
	/*sf::View minimapView;
	minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));
	window->setView(minimapView);*/
	/*window->draw(aiMove_msg);
	window->draw(depth_msg);
	window->draw(eval_msg);*/
	window->display();
}

bool AiGame::PlayerTurn(sf::Vector2i indexes)
{
	if (pieces[indexes.x][indexes.y]->IsWhite())
		return true;
	return false;
}

int AiGame::evaluate()
{
	int metirial = 0, mul;
	int piecePositionScore = 0;
	int isEndGame = IsEndGame();
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (!pieces[i][j]) continue;
			std::string strType = pieces[i][j]->GetType();
			if (pieces[i][j]->GetType()[1] == 'K')
				if (isEndGame)
					strType.push_back('E');
				else
					strType.push_back('M');
			piecePositionScore += piecePositionScores[strType][i][j];
			mul = pieces[i][j]->IsWhite() ? -1 : 1;
			metirial += mul * (scores[strType[1]] + piecePositionScore);
			piecePositionScore = 0;
		}
	}
	if (!isEndGame)
		return metirial;
	///////////////end game
	int whiteEval = 0;
	int blackEval = 0;
	auto kingsLocations = GetKingsLocation();
	whiteEval += EndGameEval(kingsLocations[0], kingsLocations[1], 1);
	blackEval += EndGameEval(kingsLocations[1], kingsLocations[0], 1);
	metirial += blackEval - whiteEval;
	return metirial;
}

int AiGame::EndGameEval(sf::Vector2i friendlyKingSqure, sf::Vector2i opponentKingSqure, float endGameWeight)
{
	
	int evaluation = 0;
	// Favour positions when opponent king has been forces away from the center
	int opponentKingRank = opponentKingSqure.y;
	int opponentKingFile = opponentKingSqure.x;

	int opponentKingDstToCenterFile = std::max(3 - opponentKingFile, opponentKingFile - 4);
	int opponentKingDstToCenterRank = std::max(3 - opponentKingRank, opponentKingRank - 4);
	evaluation += (opponentKingDstToCenterFile + opponentKingDstToCenterRank) * 100 * endGameWeight;

	int friendlyKingRank = friendlyKingSqure.y;
	int friendlyKingFile = friendlyKingSqure.x;

	int dstBetweenKingsFile = abs(friendlyKingFile - opponentKingFile);
	int dstBetweenKingsRank = abs(friendlyKingRank - opponentKingRank);
	int dstBetweenKings = dstBetweenKingsFile + dstBetweenKingsRank;
	evaluation += (14 - dstBetweenKings) * 200 * endGameWeight;
	return (int)(evaluation);
	
}



int AiGame::minimax(int depth, int isMax)
{
	if (depth == 0)
		return evaluate();
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = AllLegalMoves(!isMax);
	if (isMax)
	{
		int bestEval = -INFINITY;
		if (moves.size() == 0) {
			if (InCheck())
				return bestEval;
			return 0;
		}
		for (auto move : moves)
		{
			// do the move :
			Move(move.first, move.second);
			int eval = minimax(depth - 1, !isMax);
			if (eval > bestEval)
				bestEval = eval;
			// undo the move :
			UndoMove(move.first, move.second);
		}
		return bestEval;
	}
	else
	{
		int bestEval = +INFINITY;
		if (moves.size() == 0) {
			if (InCheck())
				return bestEval;
			return 0;
		}
		for (auto move : moves)
		{
			// do the move :
			Move(move.first, move.second);
			int eval = minimax(depth - 1, !isMax);
			if (eval < bestEval)
				bestEval = eval;
			// undo the move :
			UndoMove(move.first, move.second);
		}
		return bestEval;
	}
	return 0;
}

int AiGame::minimax(int depth, int isMax, int alpha, int beta)
{
	if (depth == 0) {
		int val = evaluate();
		return val;
	}
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = AllLegalMoves(!isMax);

	if (isMax)
	{
		int bestEval = -INFINITY;
		if (moves.size() == 0) {
			if (InCheck())
				return bestEval;
			return 0;
		}
		for (auto move : moves)
		{
			// do the move :
			Move(move.first, move.second);
			int eval = minimax(depth - 1, !isMax, alpha, beta);
			bestEval = std::max(bestEval, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) {
				// undo the move :
				UndoMove(move.first, move.second);
				break;
			}	
			// undo the move :
			UndoMove(move.first, move.second);
		}
		return bestEval;
	}
	else
	{
		int bestEval = +INFINITY;
		if (moves.size() == 0) {
			if (InCheck())
				return bestEval;
			return 0;
		}
		for (auto move : moves)
		{
			// do the move :
			Move(move.first, move.second);
			int eval = minimax(depth - 1, !isMax, alpha, beta);
			bestEval = std::min(bestEval, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha) {
				// undo the move :
				UndoMove(move.first, move.second);
				//WriteHashEntry(bestEval, depth, hashBETA);
				break;
			}
			// undo the move :
			UndoMove(move.first, move.second);
		}
		return bestEval;
	}
}


	
std::pair<sf::Vector2i, sf::Vector2i> AiGame::findBestMove()
{
	auto moves = AllLegalMoves(false);
	std::pair<sf::Vector2i, sf::Vector2i> bestMove;
	int bestScore = -INFINITY;
	for (auto move : moves)
	{
		if (lastMoveVec.size() > 5 && lastMoveVec[lastMoveVec.size() - 4] == move && lastMoveVec[lastMoveVec.size()-5] == lastMoveVec[lastMoveVec.size() - 1])
			continue;
		/*if (lastMoveVec.size() > 8 && lastMoveVec[lastMoveVec.size() - 8] == move)
			continue;*/
		Move(move.first, move.second);
		whiteTurn = !whiteTurn;
		int scoremini = minimax(DEPTH - 1, 0, -INFINITY, INFINITY);
		//int scorenega = NegaMax(DEPTH - 1, true, -INFINITY, INFINITY);
		/*std::cout << "score mini :" << scoremini << std::endl;
		std::cout << "score nega :" << scorenega << std::endl;*/
		whiteTurn = !whiteTurn;
		if (scoremini > bestScore)
		{
			bestScore = scoremini;
			bestMove = move;
		}
		UndoMove(move.first, move.second);
	}
	a = 0;
	/*eval_msg.setString("eval: " + bestScore);
	depth_msg.setString("depths searched: " + DEPTH);*/
	return bestMove;
}

void AiGame::SmartAi()
{
	auto move = findBestMove();
	Move(move.first, move.second);
	std::string t = TranslateMove(move);
	aiMove_msg.setString("ai move: " + t);
	
}

int AiGame::MoveGenerarionTest(int depth, int isWhite, int startDepth)
{
	if (depth == 0)
		return 1;
	auto moves = AllLegalMoves(isWhite);
	//moves = OrderMoves(moves);
	int numPositions = 0;
	int numPositionsEachMove = 0;
	std::string str;
	for (auto move : moves)
	{
		/*if (depth == startDepth) {
			if(TranslateMove(move) == )
		}*/
		Move(move.first, move.second);
		whiteTurn = !whiteTurn;
		numPositionsEachMove = MoveGenerarionTest(depth - 1, !isWhite, startDepth);
		whiteTurn = !whiteTurn;
		numPositions += numPositionsEachMove;
		str = TranslateMove(move);
		if (depth == startDepth) {
			// check if the move was promotion and calc for each option.
			if (moveTypeVec.back() == PROMOTION) {
				char t = pieces[move.second.x][move.second.y]->GetType()[1];
				str.push_back(t);
			}
			moveCounter[str] = numPositionsEachMove;
			std::cout << str << " : " << numPositionsEachMove << std::endl;
		}
		if (moveTypeVec.back() == PROMOTION)
		{
			UndoMove(move.first, move.second);
			for (int i = 0; i < 3; i++)
			{
				promotionTypeVec.push_back(i + 1);
				Move(move.first, move.second);
				whiteTurn = !whiteTurn;
				numPositionsEachMove = MoveGenerarionTest(depth - 1, !isWhite, startDepth);
				whiteTurn = !whiteTurn;
				numPositions += numPositionsEachMove;
				if (depth == startDepth) {
					// check if the move was promotion and calc for each option.
					str = TranslateMove(move);
					if (moveTypeVec.back() == PROMOTION) {
						char t = pieces[move.second.x][move.second.y]->GetType()[1];
						str.push_back(t);
					}
					moveCounter[str] = numPositionsEachMove;
					std::cout << str << " : " << numPositionsEachMove << std::endl;
				}
				UndoMove(move.first, move.second);
				promotionTypeVec.pop_back();
			}

		}
		else
			UndoMove(move.first, move.second);


	}
	return numPositions;
}

int AiGame::Search(int depth, bool isWhite)
{
	if (depth == 0)
		return evaluate();
	auto moves = AllLegalMoves(isWhite);
	
	if (moves.size() == 0) {
		if (InCheck())
			return -INFINITY;
		return 0;
	}
	std::pair<sf::Vector2i, sf::Vector2i> bestMove = moves[0];
	int bestEvaluation = -INFINITY;
	for (auto move : moves)
	{
		Move(move.first, move.second);
		int evaluation = -Search(depth - 1, !isWhite);
		bestEvaluation = std::max(evaluation, bestEvaluation);
		UndoMove(move.first, move.second);
	}

	return bestEvaluation;
}

int AiGame::NegaMax(int depth,bool isWhite, int alpha, int beta)
{
	if (!depth)
		return evaluate();
	auto moves = AllLegalMoves(isWhite);
	if (!moves.size()) {
		if (InCheck())
			return -INFINITY; // checkmate is the worth position
		return 0; // draw is equal to zero.
	}
	int bestEval = -INFINITY;
	for (auto move : moves)
	{
		Move(move.first, move.second);
		whiteTurn = !whiteTurn;
		int score = -NegaMax(depth - 1, !isWhite, -beta, -alpha);
		whiteTurn = !whiteTurn;
		UndoMove(move.first, move.second);
		bestEval = std::max(bestEval, score);
		//if (score >= beta)
		//	return beta; // *snif*
		//alpha = std::max(alpha, score);
	}
	//return alpha;
	return bestEval;
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> AiGame::OrderMoves(std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves)
{
	if (!moves.size() || moves.size() == 1)
		return moves;
	std::vector<int> scoreGuesses;
	for (auto move : moves)
	{
		int moveScoreGuess = 0;
		sf::Vector2i movePieceIndexes = move.first;
		sf::Vector2i capturedPieceIndexes = move.second;
		Piece* movePiece = pieces[movePieceIndexes.x][movePieceIndexes.y];
		Piece* capturedPiece = pieces[capturedPieceIndexes.x][capturedPieceIndexes.y];
		if (capturedPiece)
			moveScoreGuess = 10 * scores[capturedPiece->GetType()[1]] - scores[movePiece->GetType()[1]];
		if (movePiece->GetType()[1] == 'p')
			if (movePiece->IsWhite() && capturedPieceIndexes.x == 0)
				moveScoreGuess += scores['Q'];
			else if(!movePiece->IsWhite() && capturedPieceIndexes.x == DIMENSIONS-1)
				moveScoreGuess += scores['Q'];
		Move(move.first, move.second);
		if (squreUnderAttack(capturedPieceIndexes))
			moveScoreGuess -= scores[movePiece->GetType()[1]];
		UndoMove(move.first, move.second);
		scoreGuesses.push_back(moveScoreGuess);
	}
	// sort the moves by there move score guesses:
	// bubble sort :
	for (int i = 0; i < scoreGuesses.size()-1; i++)
	{
		for (int j = 0; j < scoreGuesses.size() - 1 - i; j++)
		{
			if (scoreGuesses[j] < scoreGuesses[j + 1])
			{
				std::swap(moves[j], moves[j + 1]);
			}
		}
	}
	return moves;
}


std::vector<std::vector<int>> AiGame::FlipTableScores(std::vector<std::vector<int>> tableScores)
{
	std::vector<std::vector<int>> flipedTable;
	std::vector<int> flipedTableLine;
	for (int i = DIMENSIONS - 1; i >= 0; i--)
	{
		flipedTableLine.clear();
		for (int j = DIMENSIONS - 1; j >= 0; j--)
		{
			flipedTableLine.push_back(tableScores[i][j]);
		}
		flipedTable.push_back(flipedTableLine);
	}
	return flipedTable;
}

unsigned long AiGame::Random32bit()
{
	// get current state
	unsigned long number = state;

	// XOR shift
	number ^= number << 13;
	number ^= number >> 17;
	number ^= number << 5;

	// update random number state
	state = number;

	return number;
}

unsigned __int64 AiGame::Random64bit()
{
	unsigned __int64 n1, n2, n3, n4;
	n1 = (Random32bit()) & 0xFFFF;
	n2 = (Random32bit()) & 0xFFFF;
	n3 = (Random32bit()) & 0xFFFF;
	n4 = (Random32bit()) & 0xFFFF;
	return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);	
}

void AiGame::init_zorbist()
{
	std::vector<std::string> piece_types = { "wp", "wR", "wN", "wB", "wQ", "wK","bp", "bR", "bN", "bB", "bQ", "bK" };
	int i = 0;
	for (auto p : piece_types)
		piece_key_value[i++] = p;
	for (i = 0; i < NUM_OF_PIECES; i++)
	{
		for (int j = 0; j < DIMENSIONS * DIMENSIONS; j++)
		{
			pieces_keys[i][j] = Random64bit();
		}
	}
	side_key = Random64bit();
	ClearTranspositionTable();
}

unsigned __int64 AiGame::ZorbistKey()
{
	unsigned __int64 final_key = 0;
	for (int i = 0; i < NUM_OF_PIECES; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			for (int k = 0; k < DIMENSIONS; k++)
			{
				if (!pieces[j][k])
					continue;
				if(pieces[j][k]->GetType() == piece_key_value[i])
					final_key ^= pieces_keys[i][j * DIMENSIONS + k];
			}
		}	
	}
	if (!whiteTurn)
		final_key ^= side_key;
	return final_key;
}

void AiGame::ClearTranspositionTable()
{
	for (int i = 0; i < hash_size; i++)
	{
		ttTable[i].depth = 0;
		ttTable[i].flag = 0;
		ttTable[i].hash_key = 0;
		ttTable[i].score = 0;
	}
}

void AiGame::WriteHashEntry(int score, int depth, int hash_flag)
{
	__int64 hash_key = ZorbistKey();
	__int64 hash_index = hash_key % hash_size;
	ttTable[abs(hash_index)] = TranspositionTable(hash_key, depth, hash_flag, score);
}

unsigned __int64 AiGame::ReadHashEntry(int alpha, int beta, int depth)
{
	unsigned __int64 hash_key = ZorbistKey();
	TranspositionTable* hash_entry = &ttTable[hash_key % hash_size];
	if (hash_entry && hash_entry->hash_key == hash_key)
		if (hash_entry->depth >= depth)
		{
			int flag = hash_entry->flag;
			if (flag == hashEXACT)
				return hash_entry->score;
			if (flag == hashALPHA && hash_entry->score <= alpha)
				return alpha;
			if (flag == hashBETA && hash_entry->score >= beta)
				return beta;
		}
	return no_hash_entry;
}










float AiGame::CalcEndGameWeight(int materialCountWithoutPawns, int endGameMaterialStart)
{
	float m = 0.1f * (DIMENSIONS + 1 - materialCountWithoutPawns);
	return m + 0.1;
	/*const float m = 1 / endGameMaterialStart;
	float min = materialCountWithoutPawns * m < 1 ? materialCountWithoutPawns * m : 1;
	return 1 - min;*/
}

int AiGame::CountMaterial(bool isWhite)
{
	int material = 0;
	char color = isWhite ? 'w' : 'b';
	std::string type;
	type = color;
	material += CountPieceType(type + 'p') /** scores['p']*/; // pown material
	material += CountPieceType(type + 'B') /** scores['B']*/; // Bishop material
	material += CountPieceType(type + 'Q') /** scores['Q']*/; // Queen material
	material += CountPieceType(type + 'N') /** scores['N']*/; // Knight material
	material += CountPieceType(type + 'R') /** scores['R']*/; // Rock material
	return material;
}



int AiGame::CountPieceType(std::string type)
{ 
	int count = 0;
	for (int i = 0; i < DIMENSIONS; i++)
	{
		for (int j = 0; j < DIMENSIONS; j++)
		{
			if (pieces[i][j] && pieces[i][j]->GetType() == type)
				count += 1;	
		}
	}
	return count;
}

int AiGame::IsEndGame()
{
	return CountMaterial(true) < 7 && CountMaterial(false) < 7;
}

/*

	things to do :
	make promotion choice.
	make move ordering for pure alpha beta pruning
	make a search instead of two functions
	make getlegalmoves better
	make all the thigs coding adventure did
*/