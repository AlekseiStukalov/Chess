#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "DeckCell.h"

class IDeckCell;
class DeckCell;

//sizeof = 40(24)
class Chessman : IChessman
{
public:
    Chessman();
    Chessman(ChessColor nChessmanColor, ChessmanValue nChessmanValue);
    ~Chessman();

    virtual IDeckCell* GetCurrentCell() const;
    virtual ChessColor GetChessmanColor() const;
    virtual ChessmanValue GetChessmanValue() const;

    void SetCurrentCell(DeckCell *pCell);
    void SetChessmanColor(ChessColor nColor);
    void SetChessmanValue(ChessmanValue nValue);
    size_t GetChessmanStepNumber() const;
    size_t GetChessmanlastUsedStepNumber() const;

    void IncChessmanStepNumber();
    void SetChessmanLastUsed(size_t lastUsed);

protected:

    DeckCell *m_pCurrentCell;

    ChessColor m_nChessmanColor;
    ChessmanValue m_nChessmanValue;

    size_t m_nChessmanStepNumber;
    size_t m_nLastUsedStepNumber;
};