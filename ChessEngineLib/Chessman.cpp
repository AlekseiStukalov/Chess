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
    m_bInitState = true;
    m_bCanDoCastling = false;
    m_bKilled = false;

    if (nChessmanValue == FigureKing || nChessmanValue == FigureBoat)
        m_bCanDoCastling = true;
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

bool Chessman::IsInitState()
{
    return m_bInitState;
}

bool Chessman::IsKilled()
{
    return m_bKilled;
}

bool Chessman::CanDoCastling()
{
    return m_bCanDoCastling;
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

void Chessman::SetCanDoCastling(bool bCastling)
{
    m_bCanDoCastling = bCastling;
}

void Chessman::SetKilled()
{
    m_bKilled = true;
}

void Chessman::ResetInitState()
{
    m_bInitState = false;
}
