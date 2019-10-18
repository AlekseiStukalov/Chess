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
    m_bKilled = false;
    m_nChessmanStepNumber = 0;
}


Chessman::~Chessman()
{
    m_pCurrentCell = nullptr;
}

IDeckCell* Chessman::GetCurrentCell()
{
    return m_pCurrentCell;
}

ChessColor Chessman::GetChessmanColor()
{
    return m_nChessmanColor;
}

ChessmanValue Chessman::GetChessmanValue()
{
    return m_nChessmanValue;
}

bool Chessman::IsKilled()
{
    return m_bKilled;
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

void Chessman::SetKilled()
{
    m_bKilled = true;
}

int Chessman::GetChessmanStepNumber()
{
    return m_nChessmanStepNumber;
}

void Chessman::IncChessmanStepNumber()
{
    m_nChessmanStepNumber++;
}
