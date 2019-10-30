#pragma once
#pragma warning(disable : 4710)

#include "Includes.h"

//Steps notation
#define STEP_QUIET "-"
#define STEP_KILL "x"
#define STEP_CHECK "+"
#define STEP_CHECKMATE "#"
#define STEP_EN_PASSAN "e.p."
#define STEP_STANDOFF "="
#define STEP_CASTLING_A "0-0-0"
#define STEP_CASTLING_H "0-0"
#define STEP_PAWN_RESPAWN "RESP:"
#define SEPARATOR "%"


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
        bSpecialStep = false;
        threatToWhiteKing = None;
        threatToBlackKing = None;
    }
    StepResult(const StepResult &obj) { operator=(obj); }
    StepResult& operator= (const StepResult &obj)
    {
        threatToBlackKing = obj.threatToBlackKing;
        threatToWhiteKing = obj.threatToWhiteKing;
        respawnCellName = obj.respawnCellName;
        gameState = obj.gameState;
        nError = obj.nError;
        nNextPlayerColor = obj.nNextPlayerColor;
        bSpecialStep = obj.bSpecialStep;
        return *this;
    }

    ThreatToKing threatToWhiteKing;
    ThreatToKing threatToBlackKing;

    std::string respawnCellName;
    GameState gameState;
    MakeStepError nError;

    ChessColor nNextPlayerColor;

    bool bSpecialStep;
};

struct CellPos
{
    CellPos() : Number(0), LiterNumber(0) {}
    CellPos(int number, int literNumber) : Number(number), LiterNumber(literNumber) {}
    CellPos(const CellPos &obj) { operator=(obj); }
    CellPos& operator=(const CellPos &obj)
    {
        Number = obj.Number;
        LiterNumber = obj.LiterNumber;
        return *this;
    }

    bool operator==(const CellPos &obj)
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
    StepsPossibility() {};
    StepsPossibility& operator+= (StepsPossibility &obj)
    {
        AddNewPositions(CanKill, obj.CanKill);
        AddNewPositions(CanStep, obj.CanStep);
        AddNewPositions(CanProtect, obj.CanProtect);
        AddNewPositions(AdditionalCells, obj.AdditionalCells);

        return *this;
    }

    bool Empty()
    {
        return (
            CanKill.empty() && CanStep.empty() &&
            CanProtect.empty() && AdditionalCells.empty());
    }

    void Clear()
    {
        CanKill.clear();
        CanStep.clear();
        CanProtect.clear();
        AdditionalCells.clear();
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
        for (size_t i = 0; i < src.size(); i++)
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
    CellPos chessmanPos;
    ChessmanValue chessmanValue;
    StepsPossibility steps;
};


class AllSteps
{
public:
    std::vector<CellPos> GetChessmenWhoCanAttackCell(CellPos &targetPos)
    {
        std::vector<CellPos> cells;

        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanKill, targetPos))
                cells.push_back(chessmanSteps.chessmanPos);
        }

        return cells;
    }

    std::vector<CellPos> GetChessmenWhoCanStepToCell(CellPos &targetPos)
    {
        std::vector<CellPos> cells;

        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanStep, targetPos))
                cells.push_back(chessmanSteps.chessmanPos);
        }

        return cells;
    }

    std::vector<CellPos> GetChessmenWhoCanProtectCell(CellPos &targetPos)
    {
        std::vector<CellPos> cells;

        for (auto &chessmanSteps : m_AllSteps)
        {
            if (chessmanSteps.steps.IsPosExists(chessmanSteps.steps.CanProtect, targetPos))
                cells.push_back(chessmanSteps.chessmanPos);
        }

        return cells;
    }

    void AddChessmanSteps(ChessmanPossibleSteps &steps)
    {
        m_AllSteps.push_back(steps);
    }

private:

    std::vector<ChessmanPossibleSteps> m_AllSteps;
};


