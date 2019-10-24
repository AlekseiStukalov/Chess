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
    virtual IDeckCell* GetCurrentCell() const = 0;
    virtual ChessColor GetChessmanColor() const = 0;
    virtual ChessmanValue GetChessmanValue() const = 0;
};

class IDeckCell
{
public:
    virtual ChessColor GetCellColor() const = 0;
    virtual IChessman* GetChessman() const = 0;
    virtual std::string GetCellName() const = 0;
    virtual CellPos GetCellPos() const = 0;
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
    virtual bool GoToPreviousStep() = 0;
    virtual bool GoToNextStep() = 0;
};