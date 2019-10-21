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
    void ShowStats();
    void ShowTurn(ChessColor currentPlayerColor);
    bool GetStep(std::string &stepStr);
    void ShowError(MakeStepError nError);

private:

    ConsoleUIHelper m_uiHelper;
    IDeck *m_pDeck;
    Player m_WhitePlayer;
    Player m_BlackPlayer;
};

