#include "stdafx.h"
#include "Chessman.h"

Chessman::Chessman()
{
    m_pCurrentCell = nullptr;
}

Chessman::Chessman(ChessColor nChessmanColor, ChessmanValue nChessmanValue)
{
    m_pCurrentCell = nullptr;
    m_nChessmanColor = nChessmanColor;
    m_nChessmanValue = nChessmanValue;
    m_nChessmanStepNumber = 0;
}

Chessman::~Chessman()
{
    m_pCurrentCell = nullptr;
}

IDeckCell* Chessman::GetCurrentCell() const
{
    return m_pCurrentCell;
}

ChessColor Chessman::GetChessmanColor() const
{
    return m_nChessmanColor;
}

ChessmanValue Chessman::GetChessmanValue() const
{
    return m_nChessmanValue;
}

void Chessman::SetCurrentCell(DeckCell *pCell)
{
    m_pCurrentCell = pCell;
}

void Chessman::SetChessmanColor(ChessColor nColor)
{
    m_nChessmanColor = nColor;
}

void Chessman::SetChessmanValue(ChessmanValue nValue)
{
    m_nChessmanValue = nValue;
}

size_t Chessman::GetChessmanStepNumber() const
{
    return m_nChessmanStepNumber;
}

void Chessman::IncChessmanStepNumber()
{
    m_nChessmanStepNumber++;
}

void Chessman::SetChessmanStepNumber(size_t stepNumber)
{
    m_nChessmanStepNumber = stepNumber;
}

size_t Chessman::GetChessmanlastUsedStepNumber() const
{
    return m_nLastUsedStepNumber;
}

void Chessman::SetChessmanLastUsed(size_t lastUsed)
{
    m_nLastUsedStepNumber = lastUsed;
}
