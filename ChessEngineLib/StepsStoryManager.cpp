#include "stdafx.h"
#include "StepsStoryManager.h"
#include "DeckEngine.h"
#include "EngineHelper.h"

StepsStoryManager::StepsStoryManager()
{
    m_pDeckEngine = nullptr;
    m_nKilledChessmen = 0;
}

StepsStoryManager::~StepsStoryManager()
{
    m_pDeckEngine = nullptr;
}

void StepsStoryManager::Initialize(DeckEngine *pDeck)
{
    m_pDeckEngine = pDeck;
}

std::list<StepStoryRecord> StepsStoryManager::GetStory()
{
    return m_Steps;
}

void StepsStoryManager::AddStep(StepResult &result, std::string &sOldPos, std::string &sNewPos, Deck &deck)
{
    IChessman *pChessman = m_pDeckEngine->GetCell(sNewPos)->GetChessman();

    ChessColor playerColor = pChessman->GetChessmanColor();
    ChessColor oppositeColor = (playerColor == CHESS_COLOR_BLACK ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK);

    bool bKill = false;
    size_t totalDeadChessmen = m_pDeckEngine->GetKilledChessmen(oppositeColor).size() + m_pDeckEngine->GetKilledChessmen(playerColor).size();
    if (totalDeadChessmen != m_nKilledChessmen)
    {
        bKill = true;
        m_nKilledChessmen = totalDeadChessmen;
    }

    StepStoryRecord record;
    record.m_StepResult = result;
    record.m_nStepNumber = m_pDeckEngine->GetStepNumber();
    record.m_Deck = std::move(deck);


    record.m_sStepName += sOldPos;
    record.m_sStepName += SEPARATOR;

    if (pChessman->GetChessmanValue() == FigureKing && result.bSpecialStep)
    {
        if (m_pDeckEngine->GetCell(sNewPos)->GetCellPos().LiterNumber == 3)
            record.m_sStepName += STEP_CASTLING_A;
        else
            record.m_sStepName += STEP_CASTLING_H;

        record.m_sStepName += SEPARATOR;
    }
    else
    {
        record.m_sStepName += bKill ? STEP_KILL : STEP_QUIET;
        record.m_sStepName += SEPARATOR;
        record.m_sStepName += sNewPos;

        if (pChessman->GetChessmanValue() == FigurePawn && result.bSpecialStep)
        {
            record.m_sStepName += SEPARATOR;
            record.m_sStepName += STEP_EN_PASSAN;
        }

        if (!result.respawnCellName.empty())
        {
            record.m_sStepName += SEPARATOR;
            record.m_sStepName += STEP_PAWN_RESPAWN;
        }

        if (result.gameState == BlackWon || result.gameState == WhiteWon)
        {
            record.m_sStepName += SEPARATOR;
            record.m_sStepName += STEP_CHECKMATE;
        }
        else if (result.gameState == Standoff)
        {
            record.m_sStepName += SEPARATOR;
            record.m_sStepName += STEP_STANDOFF;
        }
        else if (result.gameState == Continue)
        {
            if (oppositeColor == CHESS_COLOR_BLACK && result.threatToBlackKing == Check ||
                oppositeColor == CHESS_COLOR_WHITE && result.threatToWhiteKing == Check)
            {
                record.m_sStepName += SEPARATOR;
                record.m_sStepName += STEP_CHECK;
            }
        }
    }

    m_Steps.push_back(record);
}

void StepsStoryManager::UpdateLastStepAfterPawnRespawn(ChessmanValue respawnChessmanValue)
{
    size_t pos = m_Steps.back().m_sStepName.find(STEP_PAWN_RESPAWN);

    if (pos != std::string::npos)
    {
        m_Steps.back().m_sStepName.insert(pos, 1, EngineHelper::Instance().GetChessmanNameByValue(respawnChessmanValue));
    }
}

StepStoryRecord* StepsStoryManager::GetStepBack(Deck& sourceDeck)
{
    auto it = std::find_if(m_Steps.begin(), m_Steps.end(), [&sourceDeck](StepStoryRecord &record) {
        return (sourceDeck.m_nStepNumber - 1) == record.m_nStepNumber;
    });

    if (it != m_Steps.end())
        return &(*it);
    else
        return nullptr;
}

StepStoryRecord* StepsStoryManager::GetStepForward(Deck& sourceDeck)
{
    auto it = std::find_if(m_Steps.begin(), m_Steps.end(), [&sourceDeck](StepStoryRecord &record) {
        return (sourceDeck.m_nStepNumber + 1) == record.m_nStepNumber;
    });

    if (it != m_Steps.end())
        return &(*it);
    else
        return nullptr;
}

StepStoryRecord* StepsStoryManager::GetStepN(size_t stepNumber)
{
    auto it = std::find_if(m_Steps.begin(), m_Steps.end(), [&stepNumber](StepStoryRecord &record) {
        return stepNumber == record.m_nStepNumber;
    });

    if (it != m_Steps.end())
        return &(*it);
    else
        return nullptr;
}
