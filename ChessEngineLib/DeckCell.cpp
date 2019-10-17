#include "stdafx.h"
#include "DeckCell.h"
#include "EngineHelper.h"

DeckCell::DeckCell()
{
    m_pChessman = nullptr;
}

DeckCell::~DeckCell()
{
    if (m_pChessman)
        delete m_pChessman;

    m_pChessman = nullptr;
}

ChessColor DeckCell::GetCellColor()
{
    return m_nCellColor;
}

IChessman* DeckCell::GetChessman()
{
    return (IChessman*)m_pChessman;
}

std::string DeckCell::GetCellName()
{
    return m_sCellName;
}

CellPos DeckCell::GetCellPos()
{
    return EngineHelper::Instance().GetCellPos(m_sCellName);
}

void DeckCell::SetCellColor(ChessColor color)
{
    m_nCellColor = color;
}

void DeckCell::SetChessman(Chessman *pChessman)
{
    m_pChessman = pChessman;
}

void DeckCell::SetCellName(std::string cellName)
{
    m_sCellName = cellName;
}
