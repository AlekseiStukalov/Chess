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
    E_NO_ROOK_CASTLING,
    E_CAN_NOT_REPLACE_THIS_CHESSMAN, //this is not a pawn
    E_WRONG_PAWN_POSITION_FOR_RESPAWN,
    E_WRONG_NEW_CHESSMAN_NAME_FOR_RESPAWN,
    E_CAN_NOT_RESPAWN_THIS_CHESSMAN, //new chessman still alive
};

enum ChessColor
{
    CHESS_COLOR_BLACK = 1,
    CHESS_COLOR_WHITE = 2
};

enum ChessmanValue
{
    FigurePawn = 1, //Пешка
    FigureBishop = 2, //Слон
    FigureKnight = 3, //Конь
    FigureRook = 4, //Ладья
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

enum ThreatToKing
{
    Check = 1,
    CheckMate,
    Stalemate,
    None
};

struct StepResult
{
    StepResult()
    {
        nError = E_NO_ERROR;
        gameState = GameState::Continue;
    }

    ThreatToKing threatToWhiteKing;
    ThreatToKing threatToBlackKing;

    std::string respawnCellName;
    GameState gameState;
    MakeStepError nError;

    ChessColor nNextPlayerColor;
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

struct StepsPossibility
{
    StepsPossibility& operator+= (StepsPossibility &obj)
    {
        AddNewPositions(this->CanKill, obj.CanKill);
        AddNewPositions(this->CanStep, obj.CanStep);
        AddNewPositions(this->CanProtect, obj.CanProtect);
        AddNewPositions(this->AdditionalCells, obj.AdditionalCells);

        return *this;
    }

    bool IsPosExists(std::vector<CellPos> &arr, CellPos newPos)
    {
        auto it = std::find_if(arr.begin(), arr.end(), [&newPos](CellPos &existedPos) {
            return existedPos == newPos;
        });

        return it != arr.end();
    }

private:
    void AddNewPositions(std::vector<CellPos> &dest, std::vector<CellPos> &src)
    {
        for (size_t i = 0; src.size(); i++)
        {
            if (!IsPosExists(dest, src[i]))
                dest.push_back(src[i]);
        }
    }

public:

    std::vector<CellPos> AdditionalCells;
    std::vector<CellPos> CanStep;
    std::vector<CellPos> CanKill;
    std::vector<CellPos> CanProtect;
};

struct ChessmanPossibleSteps
{
    CellPos cessmanPos;
    ChessmanValue chessmanValue;
    StepsPossibility steps;
};


class AllSteps
{
public:
    std::vector<CellPos> GetChessmenWhoCanStepOrKill(CellPos &targetPos)
    {
        std::vector<CellPos> cells;

        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanStep, targetPos))
                cells.push_back(chessmanSteps.cessmanPos);
        }

        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanKill, targetPos))
                cells.push_back(chessmanSteps.cessmanPos);
        }

        return cells;
    }


    bool IsContainForStep(CellPos &pos)
    {
        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanStep, pos))
                return true;
        }

        return false;
    }

    bool IsContainForKill(CellPos &pos)
    {
        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanKill, pos))
                return true;
        }

        return false;
    }

    void AddChessmanSteps(ChessmanPossibleSteps &steps)
    {
        m_AllSteps.push_back(steps);
    }

private:

    std::vector<ChessmanPossibleSteps> m_AllSteps;
};



