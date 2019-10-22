#include "stdafx.h"
#include "../Common/Structures.h"
#include "EngineHelper.h"
#include "Deck.h"


IDeck* ChessEngine::CreateDeck()
{
    return (IDeck*)(new Deck());
}


Deck::Deck()
{
    m_nStepNumber = 0;
    CreateDeck();
    FillChessmenInitState();
}

Deck::~Deck()
{
    for (auto chessman : m_KilledChessmen)
    {
        delete chessman.second;
        chessman.second = nullptr;
    }
}

StepResult Deck::MakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos)
{
    StepResult result;

    if (!EngineHelper::Instance().IsCoordinateCorrect(sOldPos))
    {
        result.nError = E_WRONG_START_CELL;
        return result;
    }

    if (!EngineHelper::Instance().IsCoordinateCorrect(sNewPos))
    {
        result.nError = E_WRONG_END_CELL;
        return result;
    }

    DeckCell *pOldCell = (DeckCell *)GetCell(sOldPos);
    DeckCell *pNewCell = (DeckCell *)GetCell(sNewPos);
    if (!pOldCell || !pNewCell)
    {
        result.nError = E_CAN_NOT_FIND_CELL;
        return result;
    }

    if (sNewPos.compare(sOldPos) == 0)
    {
        result.nError = E_START_EQUAL_END;
        return result;
    }

    IChessman* pChessman = GetCell(sOldPos)->GetChessman();
    if (!pChessman)
    {
        result.nError = E_START_CELL_HAVE_NO_CHESSMAN;
        return result;
    }

    if (pChessman->GetChessmanColor() != playerColor)
    {
        result.nError = E_CAN_NOT_MOVE_OPPOSITE_CHESSMAN;
        return result;
    }

    CellPos newCellPos = pNewCell->GetCellPos();
    CellPos oldCellPos = pOldCell->GetCellPos();

    StepsPossibility steps = GetPossibleSteps(sOldPos);

    if (steps.IsPosExists(steps.CanStep, newCellPos) || steps.IsPosExists(steps.CanKill, newCellPos))
    {
        return DoMakeStep(pOldCell, pNewCell, steps.AdditionalCells);
    }
    else
    {
        result.nError = E_CAN_NOT_MOVE_TO_CELL;
        return result;
    }
}

StepResult Deck::DoMakeStep(DeckCell *pOldCell, DeckCell *pNewCell, std::vector<CellPos> &additionalCells)
{
    StepResult result;
    Chessman* pChessman = (Chessman*)pOldCell->GetChessman();
    ChessColor playerColor = pChessman->GetChessmanColor();
    CellPos oldCellPos = pOldCell->GetCellPos();
    CellPos newCellPos = pNewCell->GetCellPos();

    CellPos cellPosKill;
    IChessman* pEnemyChessman = pNewCell->GetChessman();
    bool bKill = false;
    if (pEnemyChessman)
    {
        bKill = true;
        cellPosKill = pNewCell->GetCellPos();
    }
    else
    {
        if (!additionalCells.empty() && pChessman->GetChessmanValue() == FigurePawn) //en passan
        {
            cellPosKill.Number = oldCellPos.Number;
            cellPosKill.LiterNumber = newCellPos.LiterNumber;

            pEnemyChessman = GetCell(cellPosKill)->GetChessman();

            if (pEnemyChessman)
                bKill = true;
            else
                result.nError = E_NO_PAWN_EN_PASSAN;
        }
    }

    if (result.nError)
        return result;

    if (bKill)
    {
        RemoveFigure(cellPosKill);
        m_KilledChessmen.push_back(std::make_pair(pEnemyChessman->GetChessmanColor(), (Chessman*)pEnemyChessman));
    }

    if (!additionalCells.empty() && pChessman->GetChessmanValue() == FigureKing)
    {
        int number = (playerColor == CHESS_COLOR_WHITE ? 1 : 8);
        Chessman *pRook = nullptr;
        if (newCellPos.LiterNumber > oldCellPos.LiterNumber)
            pRook = (Chessman *)GetChessmanFromDeck(number, 8);
        else
            pRook = (Chessman *)GetChessmanFromDeck(number, 1);

        if (pRook)
        {
            CellPos oldRookPos = pRook->GetCurrentCell()->GetCellPos();
            CellPos newRookPos;

            newRookPos.Number = oldRookPos.Number;

            if (newCellPos.LiterNumber > oldCellPos.LiterNumber)
                newRookPos.LiterNumber = oldRookPos.LiterNumber - 2;
            else
                newRookPos.LiterNumber = oldRookPos.LiterNumber + 3;

            MoveFigure((DeckCell*)pRook->GetCurrentCell(), (DeckCell*)GetCell(newRookPos));
            pRook->SetChessmanLastUsed(GetStepNumber());
            pRook->IncChessmanStepNumber();
        }
        else
            result.nError = E_NO_ROOK_CASTLING;
    }

    if (!result.nError && MoveFigure(pOldCell, pNewCell))
    {
        result.nError = E_NO_ERROR;
        result.nNextPlayerColor = (playerColor == CHESS_COLOR_BLACK ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK);

        if (pChessman->GetChessmanValue() == FigurePawn)
        {
            if ((newCellPos.Number == 8 && playerColor == CHESS_COLOR_WHITE) ||
                (newCellPos.Number == 1 && playerColor == CHESS_COLOR_BLACK))
            {
                result.nNextPlayerColor = playerColor;
                result.respawnCellName = pNewCell->GetCellName();
            }
        }

        pChessman->SetChessmanLastUsed(GetStepNumber());
        pChessman->IncChessmanStepNumber();
        GetGameState(result);
        IncStepNumber();
    }

    return result;
}

StepsPossibility Deck::GetPossibleSteps(std::string &cellName)
{
    StepsPossibility steps;
    IDeckCell *pCell = GetCell(cellName);
    if (pCell)
    {
        Chessman *pChessman = (Chessman *)pCell->GetChessman();

        if (pChessman)
            steps = GetPossibleSteps(pChessman);
    }
    return steps;
}

IDeckCell* Deck::GetCell(std::string &cellName)
{
    if (cellName.size() < 2)
        return nullptr;

    CellPos cellPos = EngineHelper::Instance().GetCellPos(cellName);
    return GetCell(cellPos.Number, cellPos.LiterNumber);
}

IDeckCell* Deck::GetCell(int number, int literNumber)
{
    IDeckCell *pCell = nullptr;
    if (EngineHelper::Instance().IsCoordinateCorrect(number, literNumber))
    {
        if (m_ChessDeck.size() >= (size_t)number)
        {
            if (m_ChessDeck[number - 1].size() >= (size_t)literNumber)
                pCell = &m_ChessDeck[number - 1][literNumber - 1];
        }
    }
    return pCell;
}

IDeckCell* Deck::GetCell(CellPos &cellPos)
{
    return GetCell(cellPos.Number, cellPos.LiterNumber);
}

IChessman* Deck::GetChessmanFromDeck(int number, int literNumber)
{
    IChessman *figure = nullptr;
    if (EngineHelper::Instance().IsCoordinateCorrect(number, literNumber))
    {
        if (m_ChessDeck.size() >= (size_t)number)
        {
            if (m_ChessDeck[number - 1].size() >= (size_t)literNumber)
                figure = m_ChessDeck[number - 1][literNumber - 1].GetChessman();
        }
    }
    return figure;
}

std::vector<IChessman*> Deck::GetKilledChessmen(ChessColor playerColor)
{
    std::vector<IChessman*> chessmen;

    for (auto chessman : m_KilledChessmen)
    {
        if (chessman.first == playerColor)
            chessmen.push_back((IChessman*)chessman.second);
    }

    return chessmen;
}

StepResult Deck::PawnRespawn(ChessColor playerColor, std::string &sPos, std::string &sChessmanName)
{
    StepResult result;
    result.nNextPlayerColor = playerColor;
    result.respawnCellName = sPos;

    if (!EngineHelper::Instance().IsCoordinateCorrect(sPos))
    {
        result.nError = E_WRONG_START_CELL;
        return result;
    }

    CellPos cellPos = EngineHelper::Instance().GetCellPos(sPos);

    DeckCell *pCell = (DeckCell*)GetCell(cellPos);
    Chessman* pChessman = (Chessman*)pCell->GetChessman();
    if (pChessman)
    {
        if (pChessman->GetChessmanValue() == FigurePawn)
        {
            if ((cellPos.Number == 8 && playerColor == CHESS_COLOR_WHITE) ||
                (cellPos.Number == 1 && playerColor == CHESS_COLOR_BLACK))
            {
                ChessmanValue respawnChessmanValue;
                if (EngineHelper::Instance().GetChessmanValueByName(respawnChessmanValue, sChessmanName))
                {
                    auto it = std::find_if(m_KilledChessmen.begin(), m_KilledChessmen.end(),
                        [&playerColor, &respawnChessmanValue](auto &chessman) {
                        return (playerColor == chessman.first && respawnChessmanValue == chessman.second->GetChessmanValue());
                    });

                    if (it != m_KilledChessmen.end())
                    {
                        Chessman *pDeadChessman = (*it).second;
                        (*it).second = pChessman;
                        pCell->SetChessman(pDeadChessman);

                        result.respawnCellName.clear();
                        result.nNextPlayerColor = (playerColor == CHESS_COLOR_BLACK ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK);
                        GetGameState(result);
                    }
                    else
                        result.nError = E_CAN_NOT_RESPAWN_THIS_CHESSMAN;
                }
                else
                    result.nError = E_WRONG_NEW_CHESSMAN_NAME_FOR_RESPAWN;
            }
            else
                result.nError = E_WRONG_PAWN_POSITION_FOR_RESPAWN;
        }
        else
            result.nError = E_CAN_NOT_REPLACE_THIS_CHESSMAN;
    }
    else
        result.nError = E_START_CELL_HAVE_NO_CHESSMAN;

    return result;
}

StepsPossibility Deck::GetPossibleSteps(Chessman *pChessman)
{
    StepsPossibility steps;

    if (!pChessman)
        return steps;

    CellPos cellPos = pChessman->GetCurrentCell()->GetCellPos();

    switch (pChessman->GetChessmanValue())
    {
    case FigurePawn:
        steps = GetPossiblePawnSteps(cellPos, pChessman);
        break;
    case FigureBishop:
        steps = GetPossibleDiagonalSteps(cellPos, pChessman);
        break;
    case FigureKnight:
        steps = GetPossibleKnightSteps(cellPos, pChessman);
        break;
    case FigureRook:
        steps = GetPossibleStrightSteps(cellPos, pChessman);
        break;
    case FigureQueen:
    {
        StepsPossibility diagonalSteps = GetPossibleDiagonalSteps(cellPos, pChessman);
        steps = GetPossibleStrightSteps(cellPos, pChessman);
        steps += diagonalSteps;
        break;
    }
    case FigureKing:
    {
        StepsPossibility kingSteps = GetPossibleKingSteps(cellPos, pChessman);
        steps = GetPossibleCastlingSteps(cellPos, pChessman);
        steps += kingSteps;
        break;
    }
    default:
        break;
    }

    return steps;
}

AllSteps Deck::GetAllPossibleSteps(ChessColor nPlayerColor)
{
    AllSteps allSteps;

    IDeckCell *pCell = nullptr;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            pCell = &m_ChessDeck[i][j];
            Chessman *pChessman = (Chessman *)pCell->GetChessman();
            if (pChessman && pChessman->GetChessmanColor() == nPlayerColor)
            {
                ChessmanPossibleSteps steps;
                steps.chessmanValue = pChessman->GetChessmanValue();
                steps.steps = GetPossibleSteps(pChessman);

                allSteps.AddChessmanSteps(steps);
            }
        }
    }
    return allSteps;
}

StepsPossibility Deck::GetPossibleStrightSteps(CellPos &cellPos, Chessman *pChessman)
{
    StepsPossibility steps;
    if (!pChessman)
        return steps;

    int number = cellPos.Number;
    int literNumber = cellPos.LiterNumber;

    int offset = 1;
    DeckCell *pCell;
    ChessColor chessmanColor = pChessman->GetChessmanColor();

    bool bStepExist = true;
    bool bUp = true, bDown = true, bRight = true, bLeft = true;
    while (bStepExist)
    {
        if (bUp)
        {
            pCell = (DeckCell*)GetCell(number + offset, literNumber);
            bUp = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        if (bDown)
        {
            pCell = (DeckCell*)GetCell(number - offset, literNumber);
            bDown = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        if (bRight)
        {
            pCell = (DeckCell*)GetCell(number, literNumber + offset);
            bRight = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        if (bLeft)
        {
            pCell = (DeckCell*)GetCell(number, literNumber - offset);
            bLeft = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        bStepExist = (bUp || bDown || bLeft || bRight);
        offset++;
    }

    return steps;
}

StepsPossibility Deck::GetPossibleDiagonalSteps(CellPos &cellPos, Chessman *pChessman)
{
    StepsPossibility steps;
    if (!pChessman)
        return steps;

    int number = cellPos.Number;
    int literNumber = cellPos.LiterNumber;

    int offset = 1;
    DeckCell *pCell;
    ChessColor chessmanColor = pChessman->GetChessmanColor();

    bool bStepExist = true;
    bool bUpRight = true, bUpLeft = true, bDownRight = true, bDownLeft = true;
    while (bStepExist)
    {
        if (bUpRight)
        {
            pCell = (DeckCell*)GetCell(number + offset, literNumber + offset);
            bUpRight = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        if (bUpLeft)
        {
            pCell = (DeckCell*)GetCell(number + offset, literNumber - offset);
            bUpLeft = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        if (bDownRight)
        {
            pCell = (DeckCell*)GetCell(number - offset, literNumber + offset);
            bDownRight = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        if (bDownLeft)
        {
            pCell = (DeckCell*)GetCell(number - offset, literNumber - offset);
            bDownLeft = AddChessmanActionForCell(pCell, chessmanColor, steps);
        }

        bStepExist = (bUpRight || bUpLeft || bDownRight || bDownLeft);
        offset++;
    }

    return steps;
}

StepsPossibility Deck::GetPossibleKnightSteps(CellPos &cellPos, Chessman *pChessman)
{
    StepsPossibility steps;
    if (!pChessman)
        return steps;

    int number = cellPos.Number;
    int literNumber = cellPos.LiterNumber;

    ChessColor chessmanColor = pChessman->GetChessmanColor();

    std::vector<std::pair<int, int>> offsets; //liter, number
    offsets.push_back(std::make_pair(1, 2));
    offsets.push_back(std::make_pair(2, 1));
    offsets.push_back(std::make_pair(2, -1));
    offsets.push_back(std::make_pair(1, -2));
    offsets.push_back(std::make_pair(-1, -2));
    offsets.push_back(std::make_pair(-2, -1));
    offsets.push_back(std::make_pair(-2, 1));
    offsets.push_back(std::make_pair(-1, 2));

    for (size_t i = 0; i < offsets.size(); i++)
    {
        int numberPos = number + offsets[i].second;
        int literNumberPos = literNumber + offsets[i].first;

        DeckCell *pCell = (DeckCell*)GetCell(numberPos, literNumberPos);

        AddChessmanActionForCell(pCell, chessmanColor, steps);
    }

    return steps;
}

StepsPossibility Deck::GetPossibleCastlingSteps(CellPos &cellPos, Chessman *pKing)
{
    StepsPossibility steps;
    if (!pKing)
        return steps;

    Chessman *pRook1 = nullptr; //cell[0]
    Chessman *pRook2 = nullptr; //cell[1]

    std::vector<CellPos> cells = FindFigureOnDeck(FigureRook, pKing->GetChessmanColor());

    if (cells.size() > 0)
    {
        pRook1 = (Chessman *)GetCell(cells[0])->GetChessman();
        if (cells.size() > 1)
            pRook2 = (Chessman *)GetCell(cells[1])->GetChessman();
    }

    if (pKing && pKing->GetChessmanStepNumber() == 0)
    {
        bool bCastlingA = false;
        bool bCastlingH = false;
        IDeckCell *pCell = nullptr;
        if (pRook1 && pRook1->GetChessmanStepNumber() == 0)
        {
            int nRookLiter = pRook1->GetCurrentCell()->GetCellPos().LiterNumber;

            if (nRookLiter == 1) 
                bCastlingA = true;
            else if(nRookLiter == 8)
                bCastlingH = true;
        }

        if (pRook2 && pRook2->GetChessmanStepNumber() == 0)
        {
            int nRookLiter = pRook2->GetCurrentCell()->GetCellPos().LiterNumber;

            if (nRookLiter == 1)
                bCastlingA = true;
            else if (nRookLiter == 8)
                bCastlingH = true;
        }

        if (bCastlingA)
        {
            int literNumberBuf = 2;
            while (true)
            {
                if (literNumberBuf == 9)
                    break;

                pCell = GetCell(cellPos.Number, literNumberBuf);
                if (pCell->GetChessman())
                {
                    if (pKing == (Chessman*)pCell->GetChessman())
                    {
                        CellPos possibleStepCell = pKing->GetCurrentCell()->GetCellPos();
                        possibleStepCell.LiterNumber -= 2;

                        steps.CanStep.push_back(possibleStepCell);
                        steps.AdditionalCells.push_back(possibleStepCell);
                        break;
                    }
                    else
                        break;
                }
                literNumberBuf++;
            }
        }

        if (bCastlingH)
        {
            int literNumberBuf = 7;
            while (true)
            {
                if (literNumberBuf == 0)
                    break;

                pCell = GetCell(cellPos.Number, literNumberBuf);
                if (pCell->GetChessman())
                {
                    if (pKing == (Chessman*)pCell->GetChessman())
                    {
                        CellPos possibleStepCell = pKing->GetCurrentCell()->GetCellPos();
                        possibleStepCell.LiterNumber += 2;

                        steps.CanStep.push_back(possibleStepCell);
                        steps.AdditionalCells.push_back(possibleStepCell);
                    }
                    else
                        break;
                }
                literNumberBuf--;
            }
        }
    }

    return steps;
}

StepsPossibility Deck::GetPossiblePawnSteps(CellPos &cellPos, Chessman *pChessman)
{
    StepsPossibility steps;
    if (!pChessman)
        return steps;

    int number = cellPos.Number;
    int literNumber = cellPos.LiterNumber;
    ChessColor chessmanColor = pChessman->GetChessmanColor();
    int offset = (chessmanColor == CHESS_COLOR_BLACK ? -1 : 1);

    DeckCell *pCell = nullptr;
    pCell = (DeckCell*)GetCell(number + offset, literNumber);
    if (pCell)
    {
        if (!pCell->GetChessman())
        {
            steps.CanStep.push_back(pCell->GetCellPos());

            if (pChessman->GetChessmanStepNumber() == 0)
            {
                pCell = (DeckCell*)GetCell(number + 2 * offset, literNumber);
                if (!pCell->GetChessman())
                    steps.CanStep.push_back(pCell->GetCellPos());
            }
        }
    }

    IDeckCell *pNeighborCell = nullptr;
    IChessman *pDiagChessman = nullptr;
    pCell = (DeckCell*)GetCell(number + offset, literNumber + offset);
    if (pCell)
    {
        pDiagChessman = pCell->GetChessman();
        if (pDiagChessman)
            AddChessmanActionForCell(pCell, chessmanColor, steps);
        else
        {
            pNeighborCell = GetCell(number, literNumber + offset);
            if (IsPawnEnPassanPossible(chessmanColor, pNeighborCell))
            {
                steps.CanStep.push_back(pCell->GetCellPos());
                steps.AdditionalCells.push_back(pCell->GetCellPos());
            }
        }
    }

    pCell = (DeckCell*)GetCell(number + offset, literNumber - offset);
    if (pCell)
    {
        pDiagChessman = pCell->GetChessman();
        if (pDiagChessman)
            AddChessmanActionForCell(pCell, chessmanColor, steps);
        else
        {
            pNeighborCell = GetCell(number, literNumber - offset);
            if (IsPawnEnPassanPossible(chessmanColor, pNeighborCell))
            {
                steps.CanStep.push_back(pCell->GetCellPos());
                steps.AdditionalCells.push_back(pCell->GetCellPos());
            }
        }
    }

    return steps;
}

StepsPossibility Deck::GetPossibleKingSteps(CellPos &cellPos, Chessman *pChessman)
{
    StepsPossibility steps;
    if (!pChessman)
        return steps;

    ChessColor chessmanColor = pChessman->GetChessmanColor();
    int number = cellPos.Number;
    int literNumber = cellPos.LiterNumber;

    std::vector<std::pair<int, int>> offsets = EngineHelper::Instance().GetKingOffsets(); //number, liter

    for (size_t i = 0; i < offsets.size(); i++)
    {
        int numberPos = number + offsets[i].first;
        int literNumberPos = literNumber + offsets[i].second;

        DeckCell *pCell = (DeckCell*)GetCell(numberPos, literNumberPos);

        AddChessmanActionForCell(pCell, chessmanColor, steps);
    }

    return steps;
}

bool Deck::IsPawnEnPassanPossible(ChessColor currentPawnColor, IDeckCell *pNeighborCell)
{
    if (pNeighborCell && pNeighborCell->GetChessman())
    {
        Chessman *pNeighborChessman = (Chessman *)pNeighborCell->GetChessman();
        if (pNeighborChessman->GetChessmanValue() == FigurePawn && pNeighborChessman->GetChessmanColor() != currentPawnColor)
        {
            int lineNumber = pNeighborCell->GetCellPos().Number;
            bool bCorrectLine = false;
            if (pNeighborChessman->GetChessmanColor() == CHESS_COLOR_BLACK && lineNumber == 5)
                bCorrectLine = true;
            if (pNeighborChessman->GetChessmanColor() == CHESS_COLOR_WHITE && lineNumber == 4)
                bCorrectLine = true;

            if (pNeighborChessman->GetChessmanStepNumber() == 1 && bCorrectLine)
            {
                if ((GetStepNumber() - pNeighborChessman->GetChessmanlastUsedStepNumber()) == 1)
                    return true;
            }
        }
    }

    return false;
}





bool Deck::IsCellOnFire(AllSteps &rivalSteps, DeckCell *pCell, ChessColor playerColor)
{
    if (pCell)
        return IsCellOnFire(rivalSteps, pCell->GetCellName(), playerColor);
    return false;
}

bool Deck::IsCellOnFire(AllSteps &rivalSteps, std::string &cellName, ChessColor playerColor)
{
    if (cellName.size() < 2)
        return false;

    //if (rivalSteps.find_no_case(cellName) != std::string::npos)
    //    return true;

    //?

    return false;
}

bool Deck::IsCellOnFire(AllSteps &rivalSteps, int number, int literNumber, ChessColor playerColor)
{
    std::string cellName = EngineHelper::Instance().GetCellName(number, literNumber);

    return IsCellOnFire(rivalSteps, cellName, playerColor);
}




ThreatToKing Deck::GetThreatToKing(AllSteps &rivalSteps, AllSteps &friendSteps, ChessColor playerColor)
{
    ThreatToKing threat = ThreatToKing::None;
    IDeckCell *pKingCell = nullptr;
    Chessman *pKingChessman = nullptr;

    std::vector<CellPos> cells = FindFigureOnDeck(FigureKing, playerColor);

    if (cells.size() == 0)
        return threat;

    pKingCell = GetCell(cells[0]);
    pKingChessman = (Chessman *)pKingCell->GetChessman();

    CellPos kingPos = cells[0];
    StepsPossibility kingSteps = GetPossibleKingSteps(kingPos, pKingChessman);

    std::vector<CellPos> freeCells;

    for (size_t i = 0; i < kingSteps.CanStep.size(); i++)
    {
        bool bCellUnderAttack = false;

        if (rivalSteps.IsContainForKill(kingSteps.CanStep[i]))
            bCellUnderAttack = true;

        if (!bCellUnderAttack)
            freeCells.push_back(kingSteps.CanStep[i]);
    }

    bool bKingUnderAttack = rivalSteps.IsContainForKill(kingPos);
    if (bKingUnderAttack)
    {
        if (freeCells.size() == 0)
            threat = ThreatToKing::CheckMate;
        else
            threat = ThreatToKing::Check;
    }
    else
    {
        if (freeCells.size() == 0)
            threat = ThreatToKing::Stalemate;
        else
            threat = ThreatToKing::None;
    }

    //check: if king surrounded fully and there are friendly figures exists -> threat = None
    if (threat == ThreatToKing::Stalemate)
    {
        bool bOtherFiguresExists = (GetKilledChessmen(playerColor).size() < 15);
        bool bHaveFreeCellAround = false;

        std::vector<std::pair<int, int>> offsets = EngineHelper::Instance().GetKingOffsets(); //number, liter
        for (size_t i = 0; i < offsets.size(); i++)
        {
            IDeckCell *pCell = GetCell(kingPos.Number + offsets[i].first, kingPos.LiterNumber + offsets[i].second);
            if (pCell && !pCell->GetChessman())
            {
                bHaveFreeCellAround = true;
                break;
            }
        }

        if (!bHaveFreeCellAround && bOtherFiguresExists)
            threat = ThreatToKing::None;
    }
    //check: if anyone can protect -> threat = Check
    else if (threat == ThreatToKing::CheckMate)
    {
        //1) find who can kill king    (what if there are two killers?)
        //      If there are two possible killers ->
        //2) find all cells between king and killer
        //3) find if any of these cells exist in friendly steps (except king steps of course) and kills (include king)
    }

    return threat;
}

void Deck::GetGameState(StepResult &result)
{
    AllSteps whiteSteps = GetAllPossibleSteps(CHESS_COLOR_BLACK);
    AllSteps blackSteps = GetAllPossibleSteps(CHESS_COLOR_WHITE);

    result.threatToWhiteKing = GetThreatToKing(blackSteps, whiteSteps, CHESS_COLOR_WHITE);
    result.threatToBlackKing = GetThreatToKing(whiteSteps, blackSteps, CHESS_COLOR_BLACK);

    if (result.threatToBlackKing == ThreatToKing::Stalemate || result.threatToWhiteKing == ThreatToKing::Stalemate)
        result.gameState = GameState::Standoff;
    else if (result.threatToBlackKing == ThreatToKing::CheckMate)
        result.gameState = GameState::WhiteWon;
    else if (result.threatToWhiteKing == ThreatToKing::CheckMate)
        result.gameState = GameState::BlackWon;
    else
        result.gameState = GameState::Continue;
}

std::vector<CellPos> Deck::FindFigureOnDeck(ChessmanValue chessman, ChessColor playerColor)
{
    std::vector<CellPos> cells;

    IDeckCell *pCell = nullptr;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            IChessman *pChessman = m_ChessDeck[i][j].GetChessman();
            if (pChessman)
            {
                if (pChessman->GetChessmanColor() == playerColor && pChessman->GetChessmanValue() == chessman)
                {
                    pCell = pChessman->GetCurrentCell();
                    cells.push_back(pCell->GetCellPos());
                }
            }
        }
    }

    return cells;
}

void Deck::CreateDeck()
{
    for (int i = 1; i <= 8; i++)         //numbers
    {
        std::vector<DeckCell> cellRow;
        for (int j = 1; j <= 8; j++)     //liters
        {
            DeckCell cell;
            cell.SetCellColor(EngineHelper::Instance().GetCellColor(i, j));
            cell.SetCellInfo(EngineHelper::Instance().GetCellName(i, j));

            cellRow.push_back(cell);
        }
        m_ChessDeck.push_back(cellRow);
    }
}

void Deck::FillChessmenInitState()
{
    Chessman* pFigure = nullptr;

    std::vector<ChessmanValue> whiteChessmen = {
        FigureRook, FigureKnight, FigureBishop, FigureQueen,
        FigureKing, FigureBishop, FigureKnight, FigureRook
    };

    std::vector<ChessmanValue> blackChessmen = {
        FigureRook, FigureKnight, FigureBishop, FigureQueen,
        FigureKing, FigureBishop, FigureKnight, FigureRook
    };

    for (char i = 'A'; i <= 'H'; i++)
    {
        pFigure = new Chessman(CHESS_COLOR_WHITE, FigurePawn);
        SetFigure(pFigure, 2, i);
        pFigure = new Chessman(CHESS_COLOR_WHITE, whiteChessmen[i - 'A']);
        SetFigure(pFigure, 1, i);

        pFigure = new Chessman(CHESS_COLOR_BLACK, FigurePawn);
        SetFigure(pFigure, 7, i);
        pFigure = new Chessman(CHESS_COLOR_BLACK, blackChessmen[i - 'A']);
        SetFigure(pFigure, 8, i);
    }
}

bool Deck::SetFigure(Chessman *pChessman, int number, int literNumber)
{
    bool bRes = true;
    if (EngineHelper::Instance().IsCoordinateCorrect(number, literNumber))
    {
        DeckCell *pCell = (DeckCell*)GetCell(number, literNumber);
        pChessman->SetCurrentCell(pCell);
        pCell->SetChessman(pChessman);
    }
    else
        bRes = false;

    return bRes;
}

bool Deck::SetFigure(Chessman *pChessman, int number, char liter)
{
    return SetFigure(pChessman, number, (int)(liter - 'A' + 1));
}

Chessman* Deck::RemoveFigure(CellPos &cellPos)
{
    Chessman* pRemovingChessman = nullptr;

    if (EngineHelper::Instance().IsCoordinateCorrect(cellPos.Number, cellPos.LiterNumber))
    {
        pRemovingChessman = (Chessman*)GetChessmanFromDeck(cellPos.Number, cellPos.LiterNumber);
        DeckCell *pCell = (DeckCell*)GetCell(cellPos);
        pCell->SetChessman(nullptr);
        pRemovingChessman->SetCurrentCell(nullptr);
        pRemovingChessman->SetKilled();
    }

    return pRemovingChessman;
}

bool Deck::MoveFigure(DeckCell *pOldCell, DeckCell *pNewCell)
{
    Chessman *pChessman = (Chessman*)pOldCell->GetChessman();

    if (pChessman == nullptr)
        return false;

    if (pOldCell && pNewCell)
    {
        pOldCell->SetChessman(nullptr);
        pNewCell->SetChessman(pChessman);
        pChessman->SetCurrentCell(pNewCell);
        pChessman->IncChessmanStepNumber();
        return true;
    }

    return false;
}

bool Deck::AddChessmanActionForCell(DeckCell *pCell, ChessColor figureColor, StepsPossibility &steps)
{
    bool bCellEmpty = false;
    if (pCell)
    {
        if (pCell->GetChessman())
        {
            if (pCell->GetChessman()->GetChessmanColor() == figureColor)
                steps.CanProtect.push_back(pCell->GetCellPos());
            else
                steps.CanKill.push_back(pCell->GetCellPos());
        }
        else
        {
            steps.CanStep.push_back(pCell->GetCellPos());
            bCellEmpty = true;
        }
    }
    return bCellEmpty;
}

size_t Deck::GetStepNumber()
{
    return m_nStepNumber;
}

void Deck::IncStepNumber()
{
    m_nStepNumber++;
}
