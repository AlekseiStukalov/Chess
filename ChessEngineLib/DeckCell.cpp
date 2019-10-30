#include "stdafx.h"
#include "DeckCell.h"
#include "EngineHelper.h"

DeckCell::DeckCell()
{
    m_pChessman = nullptr;
}

DeckCell& DeckCell::operator=(const DeckCell &&obj)
{
    m_nCellColor = obj.m_nCellColor;
    m_sCellName = std::move(obj.m_sCellName);
    m_CellPos = obj.m_CellPos;
    m_pChessman = obj.m_pChessman;

    return *this;
}

DeckCell::~DeckCell()
{
    if (m_pChessman)
        delete m_pChessman;

    m_pChessman = nullptr;
}

ChessColor DeckCell::GetCellColor() const
{
    return m_nCellColor;
}

IChessman* DeckCell::GetChessman() const
{
    return (IChessman*)m_pChessman;
}

std::string DeckCell::GetCellName() const
{
    return m_sCellName;
}

CellPos DeckCell::GetCellPos() const
{
    return m_CellPos;
}

void DeckCell::SetCellColor(ChessColor color)
{
    m_nCellColor = color;
}

void DeckCell::SetChessman(Chessman *pChessman)
{
    m_pChessman = pChessman;
}

void DeckCell::SetCellInfo(std::string &cellName)
{
    m_sCellName = cellName;
    m_CellPos = EngineHelper::Instance().GetCellPos(cellName);
}
