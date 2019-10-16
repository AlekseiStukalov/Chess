#pragma once
#pragma warning(disable : 4710)

#include "Includes.h"

enum ChessColor
{
	CHESS_COLOR_BLACK = 1,
	CHESS_COLOR_WHITE = 2
};

enum ChessmanValue
{
	FigurePawn = 1, //Пешка
	FigureElephant = 2, //Слон
	FigureKnight = 3, //Конь
	FigureBoat = 4, //Ладья
	FigureQueen = 5, //Ферзь
	FigureKing = 6  //Король
};

enum GameState
{
    WhiteWon = 1,
    BlackWon = 2,
    Standoff = 3,          ///пат
    Continue = 4
};

struct StepResult
{
    StepResult()
    {
        bSuccess = false;
        gameState = GameState::Continue;
    }

    GameState gameState;

    bool bSuccess;
    int nNextPlayerColor;
};

struct CellPos
{
    int literNumber;
    int number;
};

enum ThreatToKing
{
    Check = 1,
    CheckMate,
    Stalemate,
    None
};

