#pragma once

#include "../Common/Structures.h"
#include "../Common/StringList.h"
#include "../Common/APIClasses.h"
#include "DeckCell.h"
#include "Deck.h"
#include "StepsStoryManager.h"

class DeckEngine : public IDeckEngine
{
public:
    DeckEngine();
    ~DeckEngine() {};

    virtual StepResult MakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos);
    virtual StepsPossibility GetPossibleSteps(CellPos &cellPos);///All possible steps with cells, where this figure will be under attack
    virtual IDeckCell* GetCell(std::string &cellName);
    virtual IDeckCell* GetCell(CellPos &cellPos);
    virtual IDeckCell* GetCell(int number, int literNumber);
    virtual IChessman* GetChessmanFromDeck(int number, int literNumber);
    virtual std::vector<IChessman*> GetKilledChessmen(ChessColor playerColor);
    virtual StepResult PawnRespawn(ChessColor playerColor, std::string &sPos, std::string &sChessmanName);
    virtual size_t GetStepNumber();
    virtual bool GoToPreviousStep();
    virtual bool GoToNextStep();

protected:
    StepResult DoMakeStep(DeckCell *pOldCell, DeckCell *pNewCell, std::vector<CellPos> &additionalCells);
    StepsPossibility GetPossibleSteps(Chessman *pChessman);
    AllSteps GetAllPossibleSteps(ChessColor nPlayerColor);
    StepsPossibility GetPossibleStrightSteps(CellPos &cellPos, Chessman *pChessman);
    StepsPossibility GetPossibleDiagonalSteps(CellPos &cellPos, Chessman *pChessman);
    StepsPossibility GetPossibleKnightSteps(CellPos &cellPos, Chessman *pChessman);
    StepsPossibility GetPossibleCastlingSteps(CellPos &cellPos, Chessman *pKing);
    StepsPossibility GetPossiblePawnSteps(CellPos &cellPos, Chessman *pChessman);
    StepsPossibility GetPossibleKingSteps(CellPos &cellPos, Chessman *pChessman);
    bool IsPawnEnPassanPossible(ChessColor currentPawnColor, IDeckCell *pNeighborCell);

    ThreatToKing GetThreatToKing(AllSteps &rivalSteps, AllSteps &friendSteps, ChessColor playerColor);
    void GetGameState(StepResult &result);

    std::vector<CellPos> FindFigureOnDeck(ChessmanValue chessman, ChessColor playerColor);
    std::vector<CellPos> GetConnectingCells(Chessman *pExecutor, Chessman *pGoal);

    void FillChessmenInitState();
    bool SetFigure(Chessman *pChessman, int number, int literNumber);
    bool SetFigure(Chessman *pChessman, int number, char liter);

    Chessman* RemoveFigure(CellPos &cellPos);
    bool MoveFigure(DeckCell *pOldCell, DeckCell *pNewCell);
    bool AddChessmanActionForCell(DeckCell *pCell, ChessColor figureColor, StepsPossibility &steps);

    void IncStepNumber();
    void SetDeck(Deck &deck);
    Deck GetDeckCopy();

protected:
    Deck m_Deck;

    StepsStoryManager m_StoryManager;
};

