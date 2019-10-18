#pragma once
#include "../Common/Structures.h"
#include "../Common/StringList.h"

class IDeck;
class IDeckCell;
class IChessman;

class CLASS_DECLSPEC ChessEngine
{
public:
    IDeck* CreateDeck();
};

class IChessman
{
public:
    virtual IDeckCell* GetCurrentCell() = 0;
    virtual ChessColor GetChessmanColor() = 0;
    virtual ChessmanValue GetChessmanValue() = 0;
    virtual bool IsKilled() = 0;
};

class IDeckCell
{
public:
    virtual ChessColor GetCellColor() = 0;
    virtual IChessman* GetChessman() = 0;
    virtual std::string GetCellName() = 0;
    virtual CellPos GetCellPos() = 0;
};

class IDeck
{
public:
    virtual StepResult MakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos) = 0;
    virtual stringVector GetPossibleSteps(std::string &cellName) = 0;
    virtual IDeckCell* GetCell(std::string &cellName) = 0;
    virtual IDeckCell* GetCell(CellPos &cellPos) = 0;
    virtual IDeckCell* GetCell(int number, int literNumber) = 0;
    virtual IChessman* GetChessmanFromDeck(int number, int literNumber) = 0;
};