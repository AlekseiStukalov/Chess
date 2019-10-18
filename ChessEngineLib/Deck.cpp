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
    //?
}

StepResult Deck::MakeStep(ChessColor playerColor, std::string &sOldPos, std::string &sNewPos)
{
    StepResult result;

    if (!EngineHelper::Instance().IsCoordinateCorrect(sOldPos))
        return result;

    if (!EngineHelper::Instance().IsCoordinateCorrect(sNewPos))
        return result;

    if (!GetCell(sOldPos) || !GetCell(sNewPos) || sNewPos.compare(sOldPos) == 0)
        return result;

    IChessman* pChessman = GetCell(sOldPos)->GetChessman();
    if (!pChessman)
        return result;

    if (pChessman->GetChessmanColor() != playerColor)
        return result;


    stringVector possibleSteps = GetPossibleSteps(playerColor, sOldPos);

    if (possibleSteps.find_no_case(sNewPos) != std::string::npos)
    {
        if (MoveFigure(sOldPos, sNewPos))
        {
            result.bSuccess = true;
            result.nNextPlayerColor = (playerColor == CHESS_COLOR_BLACK ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK);

            result.gameState = GetGameState();
            IncStepNumber();
        }
    }

    return result;
}

stringVector Deck::GetPossibleSteps(ChessColor playerColor, std::string &cellName)
{
    stringVector possibleSteps;
    IDeckCell *pCell = GetCell(cellName);
    if (pCell)
    {
        Chessman *pChessman = (Chessman *)pCell->GetChessman();

        if (pChessman && pChessman->GetChessmanColor() == playerColor)
            possibleSteps = GetPossibleSteps(pChessman);
    }
    return possibleSteps;
}

IDeckCell* Deck::GetCell(std::string &cellName)
{
    if (cellName.size() < 2)
        return nullptr;

    CellPos cellPos = EngineHelper::Instance().GetCellPos(cellName);
    return GetCell(cellPos.number, cellPos.literNumber);
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
    return GetCell(cellPos.number, cellPos.literNumber);
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

stringVector Deck::GetPossibleSteps(Chessman *pChessman)
{
    stringVector possibleSteps;

    if (!pChessman)
        return possibleSteps;

    CellPos cellPos = pChessman->GetCurrentCell()->GetCellPos();

    switch (pChessman->GetChessmanValue())
    {
    case FigurePawn:
        possibleSteps = GetPossiblePawnSteps(cellPos, pChessman);
        break;
    case FigureBishop:
        possibleSteps = GetPossibleDiagonalSteps(cellPos, pChessman);
        break;
    case FigureKnight:
        possibleSteps = GetPossibleKnightSteps(cellPos, pChessman);
        break;
    case FigureRook:
    {
        stringVector strightSteps = GetPossibleStrightSteps(cellPos, pChessman);
        possibleSteps = GetPossibleCastlingSteps(cellPos, pChessman);
        possibleSteps.insert(possibleSteps.end(), strightSteps.begin(), strightSteps.end());
        break;
    }
    case FigureQueen:
    {
        stringVector diagonalSteps = GetPossibleDiagonalSteps(cellPos, pChessman);
        possibleSteps = GetPossibleStrightSteps(cellPos, pChessman);
        possibleSteps.insert(possibleSteps.end(), diagonalSteps.begin(), diagonalSteps.end());
        break;
    }
    case FigureKing:
    {
        stringVector kingSteps = GetPossibleKingSteps(cellPos, pChessman);
        possibleSteps = GetPossibleCastlingSteps(cellPos, pChessman);
        possibleSteps.insert(possibleSteps.end(), kingSteps.begin(), kingSteps.end());
        break;
    }
    default:
        break;
    }

    return possibleSteps;
}

stringVector Deck::GetAllPossibleSteps(ChessColor nPlayerColor)
{
    stringVector possibleSteps;
    stringVector rivalSteps;

    IDeckCell *pCell = nullptr;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            pCell = &m_ChessDeck[i][j];
            if (pCell->GetChessman())
            {
                if (pCell->GetChessman()->GetChessmanColor() == nPlayerColor)
                {
                    stringVector buffer = GetPossibleSteps(nPlayerColor, pCell->GetCellName());
                    possibleSteps.insert(possibleSteps.end(), buffer.begin(), buffer.end());
                }
            }
        }
    }
    return possibleSteps;
}

stringVector Deck::GetPossibleStrightSteps(CellPos &cellPos, Chessman *pChessman)
{
    stringVector possibleSteps;
    if (!pChessman)
        return possibleSteps;

    int number = cellPos.number;
    int literNumber = cellPos.literNumber;

    bool bStepExist;
    int offset = 1;
    DeckCell *pCell;
    ChessColor chessmanColor = pChessman->GetChessmanColor();

    while (true)
    {
        bStepExist = false;
        pCell = (DeckCell*)GetCell(number + offset, literNumber);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        pCell = (DeckCell*)GetCell(number - offset, literNumber);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        pCell = (DeckCell*)GetCell(number, literNumber + offset);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        pCell = (DeckCell*)GetCell(number, literNumber - offset);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        offset++;
        if (!bStepExist)
            break;
    }

    return possibleSteps;
}

stringVector Deck::GetPossibleDiagonalSteps(CellPos &cellPos, Chessman *pChessman)
{
    stringVector possibleSteps;
    if (!pChessman)
        return possibleSteps;

    int number = cellPos.number;
    int literNumber = cellPos.literNumber;

    bool bStepExist;
    int offset = 1;
    DeckCell *pCell;
    ChessColor chessmanColor = pChessman->GetChessmanColor();

    while (true)
    {
        bStepExist = false;
        pCell = (DeckCell*)GetCell(number + offset, literNumber + offset);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        pCell = (DeckCell*)GetCell(number - offset, literNumber + offset);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        pCell = (DeckCell*)GetCell(number - offset, literNumber - offset);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        pCell = (DeckCell*)GetCell(number + offset, literNumber - offset);
        if (CanFigureStepToCell(pCell, chessmanColor))
        {
            possibleSteps.push_back(pCell->GetCellName());
            bStepExist = true;
        }

        offset++;
        if (!bStepExist)
            break;
    }

    return possibleSteps;
}

stringVector Deck::GetPossibleKnightSteps(CellPos &cellPos, Chessman *pChessman)
{
    stringVector possibleSteps;
    if (!pChessman)
        return possibleSteps;

    int number = cellPos.number;
    int literNumber = cellPos.literNumber;

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
        if (CanFigureStepToCell(pCell, chessmanColor))
            possibleSteps.push_back(pCell->GetCellName());
    }

    return possibleSteps;
}

stringVector Deck::GetPossibleCastlingSteps(CellPos &cellPos, Chessman *pChessman)
{
    stringVector possibleSteps;
    if (!pChessman)
        return possibleSteps;

    Chessman *pKing = nullptr;
    Chessman *pRook1 = nullptr; //cell[0]
    Chessman *pRook2 = nullptr; //cell[1]

    stringVector cells = FindFigureOnDeck(FigureRook, pChessman->GetChessmanColor());

    if (pChessman->GetChessmanValue() == FigureKing)
    {
        pKing = pChessman;
        if (cells.size() > 0)
        {
            pRook1 = (Chessman *)GetCell(cells[0])->GetChessman();
            if (cells.size() > 1)
                pRook2 = (Chessman *)GetCell(cells[1])->GetChessman();
        }
    }
    else if (pChessman->GetChessmanValue() == FigureRook)
    {
        pRook1 = (Chessman *)GetCell(cells[0])->GetChessman();

        if (cells.size() > 1)
            pRook2 = (Chessman *)GetCell(cells[1])->GetChessman();

        stringVector kingCellName = FindFigureOnDeck(FigureKing, pChessman->GetChessmanColor());
        if (kingCellName.size() > 0)
            pKing = (Chessman *)GetCell(kingCellName[0])->GetChessman();
    }

    if (pKing && pKing->GetChessmanStepNumber() == 0)
    {
        bool bCastlingA = false;
        bool bCastlingH = false;
        IDeckCell *pCell = nullptr;
        if (pRook1 && pRook1->GetChessmanStepNumber() == 0)
        {
            pCell = GetCell(cells[0]);
            if (pCell->GetCellPos().literNumber == 1)
                bCastlingA = true;
            else if(pCell->GetCellPos().literNumber == 8)
                bCastlingH = true;
        }

        if (pRook2 && pRook2->GetChessmanStepNumber() == 0)
        {
            pCell = GetCell(cells[1]);
            if (pCell->GetCellPos().literNumber == 1)
                bCastlingA = true;
            else if (pCell->GetCellPos().literNumber == 8)
                bCastlingH = true;
        }

        if (bCastlingA)
        {
            int literNumberBuf = 2;
            while (true)
            {
                if (literNumberBuf == 9)
                    break;

                pCell = GetCell(cellPos.number, literNumberBuf);
                if (pCell->GetChessman())
                {
                    if (pKing == (Chessman*)pCell->GetChessman())
                    {
                        std::string cellName;
                        if (pChessman->GetChessmanValue() == FigureKing)
                        {
                            CellPos possibleStepCell = pChessman->GetCurrentCell()->GetCellPos();
                            possibleStepCell.literNumber -= 2;
                            cellName = EngineHelper::Instance().GetCellName(possibleStepCell.number, possibleStepCell.literNumber);
                        }
                        else
                        {
                            CellPos possibleStepCell = pChessman->GetCurrentCell()->GetCellPos();
                            possibleStepCell.literNumber += 3;
                            cellName = EngineHelper::Instance().GetCellName(possibleStepCell.number, possibleStepCell.literNumber);
                        }

                        possibleSteps.push_back(cellName);
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

                pCell = GetCell(cellPos.number, literNumberBuf);
                if (pCell->GetChessman())
                {
                    if (pKing == (Chessman*)pCell->GetChessman())
                    {
                        std::string cellName;
                        if (pChessman->GetChessmanValue() == FigureKing)
                        {
                            CellPos possibleStepCell = pChessman->GetCurrentCell()->GetCellPos();
                            possibleStepCell.literNumber += 2;
                            cellName = EngineHelper::Instance().GetCellName(possibleStepCell.number, possibleStepCell.literNumber);
                        }
                        else
                        {
                            CellPos possibleStepCell = pChessman->GetCurrentCell()->GetCellPos();
                            possibleStepCell.literNumber -= 2;
                            cellName = EngineHelper::Instance().GetCellName(possibleStepCell.number, possibleStepCell.literNumber);
                        }
                        possibleSteps.push_back(cellName);
                    }
                    else
                        break;
                }
                literNumberBuf--;
            }
        }
    }

    return possibleSteps;
}

stringVector Deck::GetPossiblePawnSteps(CellPos &cellPos, Chessman *pChessman)
{
    stringVector possibleSteps;
    if (!pChessman)
        return possibleSteps;

    int number = cellPos.number;
    int literNumber = cellPos.literNumber;
    ChessColor chessmanColor = pChessman->GetChessmanColor();
    int offset = (chessmanColor == CHESS_COLOR_BLACK ? -1 : 1);

    DeckCell *pCell = nullptr;
    pCell = (DeckCell*)GetCell(number + offset, literNumber);
    if (pCell)
    {
        if (!pCell->GetChessman())
        {
            possibleSteps.push_back(pCell->GetCellName());

            if (pChessman->GetChessmanStepNumber() == 0)
            {
                pCell = (DeckCell*)GetCell(number + 2 * offset, literNumber);
                if (!pCell->GetChessman())
                    possibleSteps.push_back(pCell->GetCellName());
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
        {
            if (pDiagChessman->GetChessmanColor() != chessmanColor)
                possibleSteps.push_back(pCell->GetCellName());
        }
        else
        {
            pNeighborCell = GetCell(number, literNumber + offset);
            if (IsPawnEnPassanPossible(chessmanColor, pNeighborCell))
                possibleSteps.push_back(pNeighborCell->GetCellName());
        }
    }

    pCell = (DeckCell*)GetCell(number + offset, literNumber - offset);
    if (pCell)
    {
        pDiagChessman = pCell->GetChessman();
        if (pDiagChessman)
        {
            if (pDiagChessman->GetChessmanColor() != chessmanColor)
                possibleSteps.push_back(pCell->GetCellName());
        }
        else
        {
            pNeighborCell = GetCell(number, literNumber - offset);
            if (IsPawnEnPassanPossible(chessmanColor, pNeighborCell))
                possibleSteps.push_back(pNeighborCell->GetCellName());
        }
    }

    return possibleSteps;
}

stringVector Deck::GetPossibleKingSteps(CellPos &cellPos, Chessman *pChessman)
{
    stringVector possibleSteps;
    if (!pChessman)
        return possibleSteps;

    ChessColor chessmanColor = pChessman->GetChessmanColor();
    int number = cellPos.number;
    int literNumber = cellPos.literNumber;

    std::vector<std::pair<int, int>> offsets; //liter, number
    offsets.push_back(std::make_pair(1, 1));
    offsets.push_back(std::make_pair(1, 0));
    offsets.push_back(std::make_pair(1, -1));
    offsets.push_back(std::make_pair(0, -1));
    offsets.push_back(std::make_pair(-1, -1));
    offsets.push_back(std::make_pair(-1, 0));
    offsets.push_back(std::make_pair(-1, 1));
    offsets.push_back(std::make_pair(0, 1));

    for (size_t i = 0; i < offsets.size(); i++)
    {
        int numberPos = number + offsets[i].second;
        int literNumberPos = literNumber + offsets[i].first;

        DeckCell *pCell = (DeckCell*)GetCell(numberPos, literNumberPos);
        if (CanFigureStepToCell(pCell, pChessman->GetChessmanColor()))
            possibleSteps.push_back(pCell->GetCellName());
    }


    //?
    int oppositeColor = pChessman->GetChessmanColor() == CHESS_COLOR_BLACK ? CHESS_COLOR_WHITE : CHESS_COLOR_BLACK;


    //check for castling

    return possibleSteps;
}

bool Deck::IsPawnEnPassanPossible(ChessColor currentPawnColor, IDeckCell *pNeighborCell)
{
    if (pNeighborCell && pNeighborCell->GetChessman())
    {
        Chessman *pNeighborChessman = (Chessman *)pNeighborCell->GetChessman();
        if (pNeighborChessman->GetChessmanValue() == FigurePawn && pNeighborChessman->GetChessmanColor() != currentPawnColor)
        {
            int lineNumber = pNeighborCell->GetCellPos().number;
            bool bWasLongStep = false;
            if (pNeighborChessman->GetChessmanColor() == CHESS_COLOR_BLACK && lineNumber == 5)
                bWasLongStep = true;
            if (pNeighborChessman->GetChessmanColor() == CHESS_COLOR_WHITE && lineNumber == 4)
                bWasLongStep = true;

            if (pNeighborChessman->GetChessmanStepNumber() == 1 && bWasLongStep)
                return true;
        }
    }

    return false;
}

ThreatToKing Deck::GetThreatToKing(stringVector &rivalSteps, ChessColor playerColor)
{
    IDeckCell *pKingCell = nullptr;
    Chessman *pKingChessman = nullptr;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            IChessman *pChessman = m_ChessDeck[i][j].GetChessman();
            if (pChessman)
            {
                if (pChessman->GetChessmanColor() == playerColor && pChessman->GetChessmanValue() == FigureKing)
                {
                    pKingCell = pChessman->GetCurrentCell();
                    pKingChessman = (Chessman *)pChessman;
                }
            }
        }
    }

    stringVector kingSteps = GetPossibleKingSteps(pKingCell->GetCellPos(), pKingChessman);

    stringVector freeCells;
    for (size_t i = 0; i < kingSteps.size(); i++)
    {
        bool bCellUnderAttack = false;

        if (rivalSteps.find_no_case(kingSteps[i]) != std::string::npos)
            bCellUnderAttack = true;

        if (!bCellUnderAttack)
            freeCells.push_back(kingSteps[i]);
    }

    ThreatToKing threat;

    bool bKingUnderAttack = false;

    if (rivalSteps.find_no_case(pKingCell->GetCellName()) != std::string::npos)
        bKingUnderAttack = true;

    if (bKingUnderAttack)
    {
        if (freeCells.size() == 0)
            threat = ThreatToKing::CheckMate;
        else
            threat = ThreatToKing::Check;
    }
    else
        threat = ThreatToKing::None;

    if (threat == ThreatToKing::CheckMate)
    {
        //check: if anyone can protect -> threat = Check
    }

    return threat;
}

GameState Deck::GetGameState()
{
    GameState state;

    stringVector whiteSteps = GetAllPossibleSteps(CHESS_COLOR_BLACK);
    stringVector blackSteps = GetAllPossibleSteps(CHESS_COLOR_WHITE);

    ThreatToKing threatToWhiteKing = GetThreatToKing(blackSteps, CHESS_COLOR_WHITE);
    ThreatToKing threatToBlackKing = GetThreatToKing(whiteSteps, CHESS_COLOR_BLACK);

    if (threatToBlackKing == ThreatToKing::Stalemate || threatToWhiteKing == ThreatToKing::Stalemate)
        state = GameState::Standoff;
    else if (threatToBlackKing == ThreatToKing::CheckMate)
        state = GameState::WhiteWon;
    else if (threatToWhiteKing == ThreatToKing::CheckMate)
        state = GameState::BlackWon;
    else
        state = GameState::Continue;

    return state;
}

stringVector Deck::FindFigureOnDeck(ChessmanValue chessman, ChessColor playerColor)
{
    stringVector cells;

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
                    cells.push_back(pCell->GetCellName());
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

bool Deck::IsCellOnFire(stringVector &rivalSteps, DeckCell *pCell, ChessColor playerColor)
{
    if (pCell)
        return IsCellOnFire(rivalSteps, pCell->GetCellName(), playerColor);
    return false;
}

bool Deck::IsCellOnFire(stringVector &rivalSteps, std::string &cellName, ChessColor playerColor)
{
    if (cellName.size() < 2)
        return false;

    if (rivalSteps.find_no_case(cellName) != std::string::npos)
        return true;

    //?

    return false;
}

bool Deck::IsCellOnFire(stringVector &rivalSteps, int number, int literNumber, ChessColor playerColor)
{
    std::string cellName = EngineHelper::Instance().GetCellName(number, literNumber);

    return IsCellOnFire(rivalSteps, cellName, playerColor);
}

Chessman* Deck::RemoveFigure(std::string &cellName)
{
    if (cellName.size() < 2)
        return nullptr;

    CellPos cellPos = EngineHelper::Instance().GetCellPos(cellName);

    return RemoveFigure(cellPos.number, cellPos.literNumber);
}

Chessman* Deck::RemoveFigure(int number, int literNumber)
{
    Chessman* pRemovingChessman = nullptr;

    if (EngineHelper::Instance().IsCoordinateCorrect(number, literNumber))
    {
        pRemovingChessman = (Chessman*)GetChessmanFromDeck(number, literNumber);
        DeckCell *pCell = (DeckCell*)GetCell(number, literNumber);
        pCell->SetChessman(nullptr);
        pRemovingChessman->SetCurrentCell(nullptr);
        pRemovingChessman->SetKilled();
    }

    return pRemovingChessman;  //delete figure need later
}

bool Deck::MoveFigure(std::string &oldCellName, std::string &newCellName)
{
    if (oldCellName.size() < 2 || newCellName.size() < 2)
        return false;

    CellPos oldCellPos = EngineHelper::Instance().GetCellPos(oldCellName);
    CellPos newCellPos = EngineHelper::Instance().GetCellPos(newCellName);

    return MoveFigure(oldCellPos.number, oldCellPos.literNumber, newCellPos.number, newCellPos.literNumber);
}

bool Deck::MoveFigure(int oldNumber, int oldLiterNumber, int newNumber, int newLiterNumber)
{
    if (!EngineHelper::Instance().IsCoordinateCorrect(oldNumber, oldLiterNumber))
        return false;

    if (!EngineHelper::Instance().IsCoordinateCorrect(newNumber, newLiterNumber))
        return false;

    Chessman *pChessman = (Chessman*)GetChessmanFromDeck(oldNumber, oldLiterNumber);

    if (pChessman == nullptr)
        return false;

    DeckCell *pOldCell = nullptr, *pNewCell = nullptr;

    pOldCell = (DeckCell*)pChessman->GetCurrentCell();
    pNewCell = (DeckCell*)GetCell(newNumber, newLiterNumber);

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

bool Deck::CanFigureStepToCell(DeckCell* pCell, ChessColor figureColor)
{
    bool bRes = false;
    if (pCell)
    {
        if (pCell->GetChessman())
        {
            if (pCell->GetChessman()->GetChessmanColor() != figureColor)
                bRes = true;
        }
        else
            bRes = true;
    }
    return bRes;
}

size_t Deck::GetStepNumber()
{
    return m_nStepNumber;
}

void Deck::IncStepNumber()
{
    m_nStepNumber++;
}
