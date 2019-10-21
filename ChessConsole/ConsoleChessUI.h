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

    void MoveConsoleWindow();
    void Start();
    void ShowStats();
    void ShowTurn(ChessColor currentPlayerColor);
    std::string GetNextStepStr();
    void ShowError(MakeStepError nError);
    void ShowGameState(GameState gameState);
    std::string GetRespawnChessmanName();

private:

    ConsoleUIHelper m_uiHelper;
    IDeck *m_pDeck;
    Player m_WhitePlayer;
    Player m_BlackPlayer;
};

