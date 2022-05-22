//#include "Evaluation.h"
//
//
//
//int Evaluation::Evaluate()
//{
//	int whiteEval = 0;
//	int blackEval = 0;
//
//	int whiteMaterial = CountMaterial(true);
//	int blackMaterial = CountMaterial(false);
//
//	int whiteMaterialWithoutPawns = whiteMaterial - CountPieceType("wp", true) * pawnValue;
//	int blackMaterialWithoutPawns = blackMaterial - CountPieceType("bp", true) * pawnValue;
//	float whiteEndgamePhaseWeight = EndGamePhaseWeight(whiteMaterialWithoutPawns);
//	float blackEndgamePhaseWeight = EndGamePhaseWeight(blackMaterialWithoutPawns);
//
//	whiteEval += whiteMaterial;
//	blackEval += blackMaterial;
//	auto kingsLocations = ai.GetKingsLocation();
//	whiteEval += EndGameEval(kingsLocations[0], kingsLocations[1], whiteMaterial, blackMaterial, blackEndgamePhaseWeight);
//	blackEval += EndGameEval(kingsLocations[1], kingsLocations[0], blackMaterial, whiteMaterial, whiteEndgamePhaseWeight);
//
//	//whiteEval += EvaluatePieceSquareTables(Board.WhiteIndex, blackEndgamePhaseWeight);
//	//blackEval += EvaluatePieceSquareTables(Board.BlackIndex, whiteEndgamePhaseWeight);
//
//	
//    return blackEval - whiteEval;
//}
//
//int Evaluation::CountMaterial(bool isWhite)
//{
//	int material = 0;
//	char color = isWhite ? 'w' : 'b';
//	std::string type;
//	type = color;
//	material += CountPieceType(type + 'p', true) * pawnValue; // pown material
//	material += CountPieceType(type + 'B', true) * bishopValue; // Bishop material
//	material += CountPieceType(type + 'Q', true) * queenValue; // Queen material
//	material += CountPieceType(type + 'N', true) * knightValue; // Knight material
//	material += CountPieceType(type + 'R', true) * rookValue; // Rock material
//
//	return material;
//}
//
//float Evaluation::EndGamePhaseWeight(int materialCountWithoutPawns)
//{
//	const float m = 1 / endgameMaterialStart;
//	float min = 1 < materialCountWithoutPawns * m ? 1 : materialCountWithoutPawns * m;
//	return 1 - min;
//}
//
//
//
//int Evaluation::CountPieceType(std::string type, bool allType)
//{
//	int material = 0;
//	for (int i = 0; i < DIMENSIONS; i++)
//	{
//		for (int j = 0; j < DIMENSIONS; j++)
//		{
//			if (pieces[i][j])
//				if (allType) {
//					if (pieces[i][j]->GetType()._Equal(type))
//						material += scores[pieces[i][j]->GetType()[1]];
//				}
//				else if(pieces[i][j]->GetType()[0] == type[0])
//					material += scores[pieces[i][j]->GetType()[1]];
//		}
//	}
//	return material;
//}
//
//int Evaluation::EndGameEval(sf::Vector2i friendlyKingSqure, sf::Vector2i opponentKingSqure, int myMaterial, int opponentMaterial, float endGameWeight)
//{
//	int evaluation = 0;
//	if (myMaterial > opponentMaterial + pawnValue * 2 && endGameWeight > 0) {
//
//		int opponentKingRank = opponentKingSqure.y;
//		int opponentKingFile = opponentKingSqure.x;
//
//		int opponentKingDstToCenterFile = std::max(3 - opponentKingFile, opponentKingFile - 4);
//		int opponentKingDstToCenterRank = std::max(3 - opponentKingRank, opponentKingRank - 4);
//		evaluation += opponentKingDstToCenterFile + opponentKingDstToCenterRank;
//
//		int friendlyKingRank = friendlyKingSqure.y;
//		int friendlyKingFile = friendlyKingSqure.x;
//
//		int dstBetweenKingsFile = abs(friendlyKingFile - opponentKingFile);
//		int dstBetweenKingsRank = abs(friendlyKingRank - opponentKingRank);
//		int dstBetweenKings = dstBetweenKingsFile + dstBetweenKingsRank;
//		evaluation += 14 - dstBetweenKings;
//		return (int)(evaluation * 10 * endGameWeight);
//	}
//	return 0;
//}
