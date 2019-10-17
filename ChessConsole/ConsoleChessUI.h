#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "Player.h"
#include "ConsoleUIHelper.h"

class ConsoleChessUI
{
public:
    ConsoleChessUI();
    ~ConsoleChessUI();

    void Start();

private:

    ConsoleUIHelper m_uiHelper;
    IDeck *m_pDeck;
    Player m_WhitePlayer;
    Player m_BlackPlayer;
};

