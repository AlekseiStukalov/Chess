#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "DeckCell.h"

class IDeckCell;
class DeckCell;

class Chessman : IChessman
{
public:
    Chessman();
    Chessman(ChessColor nChessmanColor, ChessmanValue nChessmanValue);
    ~Chessman();

    virtual IDeckCell* GetCurrentCell();
    virtual ChessColor GetChessmanColor();
    virtual ChessmanValue GetChessmanValue();
    virtual bool IsKilled();

    void SetCurrentCell(DeckCell *pCell);
    void SetChessmanColor(ChessColor nColor);
    void SetChessmanValue(ChessmanValue nValue);
    void SetKilled();
    size_t GetChessmanStepNumber();
    void IncChessmanStepNumber();

    size_t GetChessmanlastUsedStepNumber();
    void SetChessmanLastUsed(size_t lastUsed);

protected:

    DeckCell *m_pCurrentCell;

    ChessColor m_nChessmanColor;
    ChessmanValue m_nChessmanValue;

    bool m_bKilled;
    size_t m_nChessmanStepNumber;
    size_t m_nLastUsedStepNumber;
};