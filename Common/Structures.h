#pragma once
#pragma warning(disable : 4710)

#include "Includes.h"

enum MakeStepError
{
    E_NO_ERROR,
    E_WRONG_START_CELL,
    E_WRONG_END_CELL,
    E_CAN_NOT_FIND_CELL,
    E_START_EQUAL_END,
    E_START_CELL_HAVE_NO_CHESSMAN,
    E_CAN_NOT_MOVE_OPPOSITE_CHESSMAN,
    E_CAN_NOT_MOVE_TO_CELL,
    E_NO_PAWN_EN_PASSAN,
    E_NOR_ROOK_CASTLING
};

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
        nError = E_NO_ERROR;
        gameState = GameState::Continue;
    }

    GameState gameState;
    MakeStepError nError;

    int nNextPlayerColor;
};

struct CellPos
{
    CellPos() : Number(0), LiterNumber(0) {}
    CellPos(int number, int literNumber) : Number(number), LiterNumber(literNumber) {}

    bool operator==(CellPos &obj)
    {
        return (
            this->Number == obj.Number
            && this->LiterNumber == obj.LiterNumber
            );
    }

    int Number;
    int LiterNumber;
};

enum ThreatToKing
{
    Check = 1,
    CheckMate,
    Stalemate,
    None
};

