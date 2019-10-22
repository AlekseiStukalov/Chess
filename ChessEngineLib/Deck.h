#pragma once

#include "../Common/Structures.h"
#include "../Common/StringList.h"
#include "../Common/APIClasses.h"
#include "DeckCell.h"

class Deck : public IDeck
{
public:
    Deck();
    ~Deck();

    virtual StepResult MakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos);
    virtual StepsPossibility GetPossibleSteps(std::string &cellName);///All possible steps with cells, where this figure will be under attack
    virtual IDeckCell* GetCell(std::string &cellName);
    virtual IDeckCell* GetCell(CellPos &cellPos);
    virtual IDeckCell* GetCell(int number, int literNumber);
    virtual IChessman* GetChessmanFromDeck(int number, int literNumber);
    virtual std::vector<IChessman*> GetKilledChessmen(ChessColor playerColor);
    virtual StepResult PawnRespawn(ChessColor playerColor, std::string &sPos, std::string &sChessmanName);

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

    void CreateDeck();
    void FillChessmenInitState();
    bool SetFigure(Chessman *pChessman, int number, int literNumber);
    bool SetFigure(Chessman *pChessman, int number, char liter);

    Chessman* RemoveFigure(CellPos &cellPos);
    bool MoveFigure(DeckCell *pOldCell, DeckCell *pNewCell);
    bool AddChessmanActionForCell(DeckCell *pCell, ChessColor figureColor, StepsPossibility &steps);

    size_t GetStepNumber();
    void IncStepNumber();

protected:
    std::vector<std::vector<DeckCell>> m_ChessDeck;     /// [number][literNumber]
    std::vector<std::pair<ChessColor, Chessman*>> m_KilledChessmen;
    size_t m_nStepNumber;
};

