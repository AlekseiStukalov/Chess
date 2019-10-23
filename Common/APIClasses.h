#pragma once
#include "../Common/Structures.h"
#include "../Common/StringList.h"

class IDeckEngine;
class IDeckCell;
class IChessman;

class CLASS_DECLSPEC ChessEngine
{
public:
    IDeckEngine* CreateDeck();
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

class IDeckEngine
{
public:
    virtual StepResult MakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos) = 0;
    virtual StepsPossibility GetPossibleSteps(std::string &cellName) = 0;
    virtual IDeckCell* GetCell(std::string &cellName) = 0;
    virtual IDeckCell* GetCell(CellPos &cellPos) = 0;
    virtual IDeckCell* GetCell(int number, int literNumber) = 0;
    virtual IChessman* GetChessmanFromDeck(int number, int literNumber) = 0;
    virtual std::vector<IChessman*> GetKilledChessmen(ChessColor playerColor) = 0;
    virtual StepResult PawnRespawn(ChessColor playerColor, std::string &sPos, std::string &sChessmanName) = 0;
    virtual size_t GetStepNumber() = 0;
};