#pragma once
#include "..\Common\Structures.h"
#include "Deck.h"

class DeckEngine;
class Deck;

class StepStoryRecord
{
public:
    StepStoryRecord() {};
    ~StepStoryRecord() {};

    size_t m_nStepNumber;
    std::string m_sStepName;   //https://en.wikipedia.org/wiki/Chess_notation
    StepResult m_StepResult;

    Deck m_Deck;
};

class StepsStoryManager
{
public:
    StepsStoryManager();
    ~StepsStoryManager();

    void Initialize(DeckEngine *pDeck);
    std::list<StepStoryRecord> GetStory();

    void AddStep(StepResult &result, std::string &sOldPos, std::string &sNewPos, Deck &deck);
    void UpdateLastStepAfterPawnRespawn(ChessmanValue respawnChessmanValue);

    StepStoryRecord* GetStepBack(Deck& sourceDeck);
    StepStoryRecord* GetStepForward(Deck& sourceDeck);
    StepStoryRecord* GetStepN(size_t stepNumber);

private:
    std::list<StepStoryRecord> m_Steps;
    DeckEngine *m_pDeckEngine;
    size_t m_nKilledChessmen;
};

