#pragma once

#include "../Common/Structures.h"

class EngineHelper
{
public:

    ChessColor GetCellColor(int number, int literNumber);
    ChessColor GetCellColor(std::string cellName);

    int GetLiterNumber(char liter);
    char GetLiterByNumber(int number);

    bool IsCoordinateCorrect(int number, int literNumber);
    bool IsCoordinateCorrect(std::string &cellName);

    std::string GetCellName(int number, int literNumber);
    CellPos GetCellPos(std::string &cellName);

    bool GetChessmanValueByName(ChessmanValue &value, std::string &name);
    char GetChessmanNameByValue(ChessmanValue &value);

    std::vector<std::pair<int, int>> GetKingOffsets();

    static EngineHelper& Instance()
    {
        static EngineHelper s;
        return s;
    }

private:
    std::string Liters;

    EngineHelper()
    {
        Liters = "ABCDEFGH";
    };

    ~EngineHelper() { };

    EngineHelper(EngineHelper const&);
    EngineHelper& operator= (EngineHelper const&) {};
};

