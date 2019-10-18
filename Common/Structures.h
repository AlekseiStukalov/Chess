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
    FigurePawn = 1, //�����
    FigureBishop = 2, //����
    FigureKnight = 3, //����
    FigureRook = 4, //�����
    FigureQueen = 5, //�����
    FigureKing = 6  //������
};

enum GameState
{
    WhiteWon = 1,
    BlackWon = 2,
    Standoff = 3,          ///���
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
    CellPos() : number(0), literNumber(0) {}
    bool operator==(CellPos &obj)
    {
        return (
            this->number == obj.number
            && this->literNumber == obj.literNumber
            );
    }

    int number;
    int literNumber;
};

enum ThreatToKing
{
    Check = 1,
    CheckMate,
    Stalemate,
    None
};

