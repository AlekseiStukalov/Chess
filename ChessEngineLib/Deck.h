#pragma once
#include "../Common/Structures.h"

class DeckCell;
class Chessman;

//sizeof = 80(40)

//Full Deck ~7kB
class Deck
{
public:
    Deck();
    ~Deck();

    Deck(const Deck& deckSrc) { operator=(deckSrc); }
    Deck& operator=(const Deck& deck);

    Deck(Deck&& deckSrc) { operator=(deckSrc); }
    Deck& operator=(Deck&& deckSrc);


    void Create();

    std::vector<std::vector<DeckCell>> ChessDeck;     /// [number][literNumber]
    std::vector<std::pair<ChessColor, Chessman*>> KilledChessmen;
    size_t m_nStepNumber;

private:
    bool bInitialized;
};