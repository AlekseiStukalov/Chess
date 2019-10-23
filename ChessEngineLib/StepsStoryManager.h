#pragma once
#include "..\Common\Structures.h"
#include "Deck.h"

class DeckEngine;
class Deck;

class StepsStoryManager
{
public:
    StepsStoryManager();
    ~StepsStoryManager();

    void Initialize(DeckEngine *pDeck);
    std::list<StepStoryRecord> GetStory();

    void AddStep(StepResult &result, std::string &sOldPos, std::string &sNewPos);
    void UpdateLastStepAfterPawnRespawn(ChessmanValue respawnChessmanValue);

    Deck* GetStepBack(Deck& sourceDeck, StepStoryRecord &stepRecord);

    //step forward

    //go to step number N

private:
    std::list<StepStoryRecord> m_Steps;
    DeckEngine *m_pDeckEngine;
    Deck *m_pBufferDeck;
    Deck *m_pOriginalDeck;
    size_t m_nKilledChessmen;
};

