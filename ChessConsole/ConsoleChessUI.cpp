#include "stdafx.h"
#include <Windows.h>
#include "ConsoleChessUI.h"
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <fcntl.h>

#include "ConsoleUIHelper.h"


ConsoleChessUI::ConsoleChessUI()
{
    m_BlackPlayer.m_nPlayerColor = CHESS_COLOR_BLACK;
    m_WhitePlayer.m_nPlayerColor = CHESS_COLOR_WHITE;

    MoveConsoleWindow();
}

ConsoleChessUI::~ConsoleChessUI()
{
}

void ConsoleChessUI::MoveConsoleWindow()
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 500, 600, TRUE);
}

void ConsoleChessUI::Start()
{
    ChessEngine engine;
    m_pDeck = engine.CreateDeck();

    //std::cout << "while player, enter name plz: ";
    //m_WhitePlayer.m_sNickname = m_uiHelper.ReadLine();

    //std::cout << "black player, enter name plz: ";
    //m_BlackPlayer.m_sNickname = m_uiHelper.ReadLine();

    std::cout << "Now lets play!";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    m_uiHelper.ClearScreen();

    ChessColor currentPlayerColor = CHESS_COLOR_WHITE;
    StepResult stepResult;

    while (true)
    {
        ShowStats();
        ShowTurn(currentPlayerColor);

        m_uiHelper.PrintDeck(m_pDeck, currentPlayerColor);

        std::string stepStr;

        if (stepResult.respawnCellName.empty())
        {
            stepStr = GetNextStepStr();
            if (stepStr.compare("q!") == 0)
                break;

            stepResult = m_pDeck->MakeStep(currentPlayerColor, stepStr.substr(0, 2), stepStr.substr(2));
        }
        else
        {
            stepStr = GetRespawnChessmanName();
            if (stepStr.compare("q!") == 0)
                break;

            stepResult = m_pDeck->PawnRespawn(currentPlayerColor, stepResult.respawnCellName, stepStr);
        }

        if (stepResult.nError)
        {
            m_uiHelper.ClearScreen();
            ShowError(stepResult.nError);
            continue;
        }

        if (stepResult.gameState != Continue)
        {
            m_uiHelper.ClearScreen();
            ShowGameState(stepResult.gameState);
            break;
        }

        currentPlayerColor = stepResult.nNextPlayerColor;
        m_uiHelper.ClearScreen();
    }
}

void ConsoleChessUI::ShowStats()
{
    std::vector<IChessman *> killedWhite = m_pDeck->GetKilledChessmen(CHESS_COLOR_WHITE);
    std::vector<IChessman *> killedBlack = m_pDeck->GetKilledChessmen(CHESS_COLOR_BLACK);

    std::cout << "White kills : ";
    for (int i = 0; i < killedBlack.size(); i++)
        std::cout << m_uiHelper.GetFigureName(killedBlack[i]->GetChessmanValue(), CHESS_COLOR_BLACK) << " ";

    std::cout << std::endl << "Black kills : ";
    for (int i = 0; i < killedWhite.size(); i++)
        std::cout << m_uiHelper.GetFigureName(killedWhite[i]->GetChessmanValue(), CHESS_COLOR_WHITE) << " ";
    std::cout << std::endl;
}

void ConsoleChessUI::ShowTurn(ChessColor currentPlayerColor)
{
    if (currentPlayerColor == CHESS_COLOR_WHITE)
        std::cout << "White [" << WHITE_CHAR_CENTER << "] ";
    else
        std::cout << "Black [ ] ";

    std::cout << "player turn:" << std::endl;
}

std::string ConsoleChessUI::GetNextStepStr()
{
    std::cout << "Enter your step (q! to exit): ";
    return m_uiHelper.ReadLine();
}

void ConsoleChessUI::ShowError(MakeStepError nError)
{
    std::string errorStr = m_uiHelper.GetErrorValueString(nError);
    if (!errorStr.empty())
        std::cout << errorStr << "!" << std::endl;
}

void ConsoleChessUI::ShowGameState(GameState gameState)
{
    switch (gameState)
    {
    case WhiteWon:
        std::cout << "White player win!";
        break;
    case BlackWon:
        std::cout << "Black player win!";
        break;
    case Standoff:
        std::cout << "Standoff!";
        break;
    default: break;
    }
}

std::string ConsoleChessUI::GetRespawnChessmanName()
{
    std::cout << "Enter Chessman name, you want to respawn (K, Q, R, N, B, P) : ";
    std::string name = m_uiHelper.ReadLine();
    return name;
}
