#include "stdafx.h"
#include "EngineHelper.h"

ChessColor EngineHelper::GetCellColor(int number, int literNumber)
{
    ChessColor cellColor;

    if (number > 0 && number <= 8 && literNumber > 0 && literNumber <= 8)
    {
        if (number % 2 == 0 && literNumber % 2 != 0)
            cellColor = CHESS_COLOR_WHITE;
        else if (number % 2 != 0 && literNumber % 2 == 0)
            cellColor = CHESS_COLOR_WHITE;
        else
            cellColor = CHESS_COLOR_BLACK;
    }

    return cellColor;
}

ChessColor EngineHelper::GetCellColor(std::string cellName)
{
    int literNumber = GetLiterNumber(cellName[0]);
    int number = (cellName[1] - '0');

    return GetCellColor(number, literNumber);
}

int EngineHelper::GetLiterNumber(char liter)
{
    int literNumber = 0;
    liter = toupper(liter);
    if (liter >= 'A' && liter <= 'H')
        literNumber = liter - 'A' + 1;

    return literNumber;
}

char EngineHelper::GetLiterByNumber(int number)
{
    return Liters[number - 1];
}

bool EngineHelper::IsCoordinateCorrect(int number, int literNumber)
{
    if (number > 0 && number <= 8 && literNumber > 0 && literNumber <= 8)
        return true;
    else
        return false;
}

bool EngineHelper::IsCoordinateCorrect(std::string &cellName)
{
    if (cellName.size() < 2)
        return false;

    CellPos cellPos = GetCellPos(cellName);
    return IsCoordinateCorrect(cellPos.Number, cellPos.LiterNumber);
}

std::string EngineHelper::GetCellName(int number, int literNumber)
{
    std::string cellName;

    cellName += GetLiterByNumber(literNumber);
    cellName += (char)(number + '0');

    return cellName;
}

CellPos EngineHelper::GetCellPos(std::string &cellName)
{
    CellPos cellPos;

    if (cellName.size() >= 2)
    {
        cellPos.Number = (cellName[1] - '0');
        cellPos.LiterNumber = EngineHelper::Instance().GetLiterNumber(cellName[0]);
    }

    return cellPos;
}

bool EngineHelper::GetChessmanValueByName(ChessmanValue &value, std::string &name)
{
    if (name.size() < 1)
        return false;

    char letter = name[0];
    letter = ::toupper(letter);

    if (letter == 'K')
        value = ChessmanValue::FigureKing;
    else if (letter == 'Q')
        value = ChessmanValue::FigureQueen;
    else if (letter == 'R')
        value = ChessmanValue::FigureRook;
    else if (letter == 'N')
        value = ChessmanValue::FigureKnight;
    else if (letter == 'B')
        value = ChessmanValue::FigureBishop;
    else if (letter == 'P')
        value = ChessmanValue::FigurePawn;
    else
        return false;

    return true;
}

std::vector<std::pair<int, int>> EngineHelper::GetKingOffsets()
{
    std::vector<std::pair<int, int>> offsets; //number, liter
    offsets.push_back(std::make_pair(1, 1));
    offsets.push_back(std::make_pair(1, 0));
    offsets.push_back(std::make_pair(1, -1));
    offsets.push_back(std::make_pair(0, 1));
    offsets.push_back(std::make_pair(0, -1));
    offsets.push_back(std::make_pair(-1, 1));
    offsets.push_back(std::make_pair(-1, 0));
    offsets.push_back(std::make_pair(-1, -1));

    return offsets;
}
