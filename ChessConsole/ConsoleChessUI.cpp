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
}

ConsoleChessUI::~ConsoleChessUI()
{
}

void ConsoleChessUI::Start()
{
    ChessEngine engine;
    m_pDeck = engine.CreateDeck();

    //std::cout << "while player, enter name plz: ";
    //m_WhitePlayer.m_sNickname = ConsoleHelper::ReadLine();

    //std::cout << "black player, enter name plz: ";
    //m_BlackPlayer.m_sNickname = ConsoleHelper::ReadLine();

    std::cout << "Now lets play!";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    m_uiHelper.ClearScreen();

    ChessColor currentPlayerColor = CHESS_COLOR_WHITE;

    while (true)
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


        if (currentPlayerColor == CHESS_COLOR_WHITE)
            std::cout << "White [" << WHITE_CHAR_CENTER << "] ";
        else
            std::cout << "Black [ ] ";

        std::cout << "player turn:" << std::endl;
        m_uiHelper.PrintDeck(m_pDeck, currentPlayerColor);

        std::cout << "Enter your step (q! to exit): ";
        std::string stepStr = m_uiHelper.ReadLine();

        if(stepStr.compare("q!") == 0)
            break;

        StepResult stepRes = m_pDeck->MakeStep(currentPlayerColor, stepStr.substr(0, 2), stepStr.substr(2));
        if (stepRes.nError)
        {
            m_uiHelper.ClearScreen();
            std::string errorStr = m_uiHelper.GetErrorValueString(stepRes.nError);
            if (!errorStr.empty())
                std::cout << errorStr << "!" << std::endl;
            continue;
        }

        if (stepRes.gameState != Continue)
        {
            break;
        }

        currentPlayerColor = currentPlayerColor == CHESS_COLOR_WHITE ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE;

        m_uiHelper.ClearScreen();
    }
}