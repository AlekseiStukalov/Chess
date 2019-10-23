#pragma once
#include "../Common/Structures.h"

class DeckCell;
class Chessman;

class Deck
{
public:
    Deck();
    ~Deck();
    Deck(const Deck& deck) { operator=(deck); }
    Deck& operator=(const Deck& deck);

    void Create();

    std::vector<std::vector<DeckCell>> ChessDeck;     /// [number][literNumber]
    std::vector<std::pair<ChessColor, Chessman*>> KilledChessmen;
    size_t m_nStepNumber;

private:
    bool bInitialized;
};