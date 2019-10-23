#include "stdafx.h"
#include "Deck.h"
#include "DeckCell.h"
#include "EngineHelper.h"


Deck::~Deck()
{
    for (auto chessman : KilledChessmen)
    {
        delete chessman.second;
        chessman.second = nullptr;
    }
}

Deck::Deck()
{
    m_nStepNumber = 0;
    bInitialized = false;
}

void Deck::Create()
{
    for (int i = 1; i <= 8; i++)         //numbers
    {
        std::vector<DeckCell> cellRow;
        for (int j = 1; j <= 8; j++)     //liters
        {
            DeckCell cell;
            cell.SetCellColor(EngineHelper::Instance().GetCellColor(i, j));
            cell.SetCellInfo(EngineHelper::Instance().GetCellName(i, j));

            cellRow.push_back(cell);
        }
        ChessDeck.push_back(cellRow);
    }

    bInitialized = true;
}

Deck& Deck::operator=(const Deck& deck)
{





    return *this;
}