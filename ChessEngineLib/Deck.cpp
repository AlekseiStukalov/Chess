#include "stdafx.h"
#include "Deck.h"
#include "DeckCell.h"
#include "EngineHelper.h"


Deck::~Deck()
{
    for (auto chessman : KilledChessmen)
    {
        if (chessman.second)
        {
            delete chessman.second;
            chessman.second = nullptr;
        }
    }
}

Deck::Deck()
{
    m_nStepNumber = 0;
    bInitialized = false;
}

Deck& Deck::operator=(const Deck& deckSrc)
{
    bInitialized = deckSrc.bInitialized;
    m_nStepNumber = deckSrc.m_nStepNumber;

    Create();

    for (size_t n = 0; n < deckSrc.ChessDeck.size(); n++)
    {
        for (size_t ln = 0; ln < deckSrc.ChessDeck[n].size(); ln++)
        {
            const DeckCell *pCell = &deckSrc.ChessDeck[n][ln];
            const Chessman *pSrcChessman = (const Chessman *)pCell->GetChessman();

            if (pSrcChessman)
            {
                Chessman *pChessman = new Chessman();
                pChessman->SetChessmanLastUsed(pSrcChessman->GetChessmanlastUsedStepNumber());
                pChessman->SetChessmanColor(pSrcChessman->GetChessmanColor());
                pChessman->SetChessmanValue(pSrcChessman->GetChessmanValue());
                pChessman->SetCurrentCell(&ChessDeck[n][ln]);

                size_t stepNumber = pSrcChessman->GetChessmanStepNumber();
                for (size_t i = 0; i < stepNumber; i++)
                    pChessman->IncChessmanStepNumber();

                ChessDeck[n][ln].SetChessman(pChessman);
            }
        }
    }

    for (size_t i = 0; i < deckSrc.KilledChessmen.size(); i++)
    {
        Chessman *pSrcChessman = deckSrc.KilledChessmen[i].second;
        Chessman *pChessman = new Chessman();
        pChessman->SetChessmanLastUsed(pSrcChessman->GetChessmanlastUsedStepNumber());
        pChessman->SetChessmanColor(pSrcChessman->GetChessmanColor());
        pChessman->SetChessmanValue(pSrcChessman->GetChessmanValue());

        size_t stepNumber = pSrcChessman->GetChessmanStepNumber();
        for (size_t i = 0; i < stepNumber; i++)
            pChessman->IncChessmanStepNumber();

        KilledChessmen.emplace_back(std::make_pair(deckSrc.KilledChessmen[i].first, pChessman));
    }

    return *this;
}

Deck& Deck::operator=(Deck&& deckSrc)
{
    bInitialized = deckSrc.bInitialized;
    m_nStepNumber = deckSrc.m_nStepNumber;

    Create();

    for (size_t n = 0; n < deckSrc.ChessDeck.size(); n++)
    {
        for (size_t ln = 0; ln < deckSrc.ChessDeck[n].size(); ln++)
        {
            DeckCell *pCell = &deckSrc.ChessDeck[n][ln];
            Chessman *pSrcChessman = (Chessman *)pCell->GetChessman();

            if (pSrcChessman)
            {
                pCell->SetChessman(nullptr);
                ChessDeck[n][ln].SetChessman(pSrcChessman);
            }
        }
    }

    for (size_t i = 0; i < deckSrc.KilledChessmen.size(); i++)
    {
        Chessman *pSrcChessman = deckSrc.KilledChessmen[i].second;
        deckSrc.KilledChessmen[i].second = nullptr;

        KilledChessmen.emplace_back(std::make_pair(deckSrc.KilledChessmen[i].first, pSrcChessman));
    }

    return *this;
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