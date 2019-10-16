#pragma once
#include "../Common/Includes.h"
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"

#define WHITE_CHAR (char)219

class ConsoleUIHelper
{
public:
	ConsoleUIHelper();
	~ConsoleUIHelper() {};

	std::string ReadLine();
	void ClearScreen();
	void PrintDeck(IDeck *pDeck, int nTargetPlayerColor);
private:
	std::string GetFigureName(ChessmanValue figureValue, int figureColor);
	void PrintEmptyCell(int cellColor);
	void PrintBottomTreshold();
	void PrintTheTopTreshold();
	void PrintChessmanCell(ChessmanValue figureValue, int figureColor, int cellColor);

	char GetLiterByNumber(int number);
	std::string GetCellName(int number, int literNumber);

	std::string Liters;
};

