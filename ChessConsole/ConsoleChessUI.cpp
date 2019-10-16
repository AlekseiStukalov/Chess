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
	std::this_thread::sleep_for(std::chrono::seconds(1));

	m_uiHelper.ClearScreen();

	ChessColor currentPlayerColor = CHESS_COLOR_WHITE;

	while (true)
	{
		if (currentPlayerColor == CHESS_COLOR_WHITE)
			std::cout << "White ";
		else
			std::cout << "Black ";

		std::cout << "player turn:" << std::endl;
		m_uiHelper.PrintDeck(m_pDeck, currentPlayerColor);

		std::cout << "Enter your step: " << std::endl;
		std::string step = m_uiHelper.ReadLine();
		std::this_thread::sleep_for(std::chrono::seconds(1));

		StepResult stepRes;

		if (step.size() > 3)
			stepRes = m_pDeck->MakeStep(CHESS_COLOR_WHITE, step.substr(0,2), step.substr(2));

		if (stepRes.gameState != Continue)
		{
			break;
		}
		m_uiHelper.ClearScreen();
		currentPlayerColor = currentPlayerColor == CHESS_COLOR_WHITE ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE;
	}
}