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
    virtual stringVector GetPossibleSteps(std::string &cellName);///All possible steps with cells, where this figure will be under attack
    virtual IDeckCell* GetCell(std::string &cellName);
    virtual IDeckCell* GetCell(CellPos &cellPos);
    virtual IDeckCell* GetCell(int number, int literNumber);
    virtual IChessman* GetChessmanFromDeck(int number, int literNumber);
    virtual std::vector<IChessman*> GetKilledChessmen(ChessColor playerColor);

protected:
    StepResult DoMakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos);
    stringVector GetPossibleSteps(Chessman *pChessman);
    stringVector GetAllPossibleSteps(ChessColor nPlayerColor);
    stringVector GetPossibleStrightSteps(CellPos &cellPos, Chessman *pChessman);
    stringVector GetPossibleDiagonalSteps(CellPos &cellPos, Chessman *pChessman);
    stringVector GetPossibleKnightSteps(CellPos &cellPos, Chessman *pChessman);
    stringVector GetPossibleCastlingSteps(CellPos &cellPos, Chessman *pChessman);
    stringVector GetPossiblePawnSteps(CellPos &cellPos, Chessman *pChessman);
    stringVector GetPossibleKingSteps(CellPos &cellPos, Chessman *pChessman);
    bool IsPawnEnPassanPossible(ChessColor currentPawnColor, IDeckCell *pNeighborCell);

    ThreatToKing GetThreatToKing(stringVector &rivalSteps, ChessColor playerColor);
    GameState GetGameState();

    stringVector FindFigureOnDeck(ChessmanValue chessman, ChessColor playerColor);

    void CreateDeck();
    void FillChessmenInitState();
    bool SetFigure(Chessman *pChessman, int number, int literNumber);
    bool SetFigure(Chessman *pChessman, int number, char liter);

    bool IsCellOnFire(stringVector &rivalSteps, DeckCell *pCell, ChessColor playerColor);
    bool IsCellOnFire(stringVector &rivalSteps, std::string &cellName, ChessColor playerColor);
    bool IsCellOnFire(stringVector &rivalSteps, int number, int literNumber, ChessColor playerColor);

    Chessman* RemoveFigure(std::string &cellName);
    Chessman* RemoveFigure(int number, int literNumber);

    bool MoveFigure(std::string &oldCellName, std::string &newCellName);
    bool MoveFigure(int oldNumber, int oldLiterNumber, int newNumber, int newLiterNumber);

    bool CanFigureStepToCell(DeckCell *pCell, ChessColor figureColor);

    size_t GetStepNumber();
    void IncStepNumber();

protected:
    std::vector<std::vector<DeckCell>> m_ChessDeck;     /// [number][literNumber]
    std::vector<std::pair<ChessColor, Chessman*>> m_KilledChessmen;
    size_t m_nStepNumber;
};

