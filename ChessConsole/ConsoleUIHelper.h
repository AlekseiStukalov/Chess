#pragma once
#include "../Common/Includes.h"
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"

#define WHITE_CHAR (char)219
#define WHITE_CHAR_CENTER (char)254

class ConsoleUIHelper
{
public:
    ConsoleUIHelper();
    ~ConsoleUIHelper() {};

    std::string GetErrorValueString(MakeStepError nError);
    std::string GetFigureName(ChessmanValue figureValue, int figureColor);
    std::string ReadLine();
    void ClearScreen();
    void PrintDeck(IDeck *pDeck, int nTargetPlayerColor);
private:
    void PrintEmptyCell(int cellColor);
    void PrintBottomTreshold();
    void PrintTheTopTreshold();
    void PrintChessmanCell(ChessmanValue figureValue, int figureColor, int cellColor);

    char GetLiterByNumber(int number);
    std::string GetCellName(int number, int literNumber);

    std::string Liters;
};

