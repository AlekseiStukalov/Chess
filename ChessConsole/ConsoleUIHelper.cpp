#include "stdafx.h"
#include "ConsoleUIHelper.h"
#include <windows.h>

ConsoleUIHelper::ConsoleUIHelper()
{
    Liters = "ABCDEFGH";
}

std::string ConsoleUIHelper::ReadLine()
{
    std::string line;
    while (true)
    {
        std::getline(std::cin, line);

        if (line.empty() || line == "\t\n" || line == "\t" || line == "\n")
        {
            std::cout << std::endl << "Empty string. Enter again: ";
            continue;
        }

        break;
    }

    return line;
}

void ConsoleUIHelper::ClearScreen()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
        )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
        )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

void ConsoleUIHelper::PrintDeck(IDeck *pDeck, int nTargetPlayerColor)
{
    PrintTheTopTreshold();

    for (int number = 1; number <= 9; number++)
    {
        if (number == 9)
        {
            for (int i = 0; i <= 40; i++)
                std::cout << "-";
            std::cout << std::endl;

            //print letters
            std::cout << " ";
            for (int i = 1; i <= 8; i++)
            {
                std::cout << "  ";
                std::cout << GetLiterByNumber(nTargetPlayerColor == CHESS_COLOR_WHITE ? i : (9 - i));
                std::cout << "  ";
            }
            std::cout << std::endl;
            break;
        }

        for (int cellLine = 0; cellLine < 3; cellLine++)
        {
            for (int literNumber = 1; literNumber <= 9; literNumber++)
            {
                if (literNumber == 9)
                {
                    //print number
                    if (cellLine == 1)
                        std::cout << " " << (nTargetPlayerColor == CHESS_COLOR_WHITE ? (9 - number) : number);

                    std::cout << std::endl;

                    break;
                }
                else
                {
                    if (literNumber == 1)
                        std::cout << "|";

                    IDeckCell *pCell = nullptr;

                    if (nTargetPlayerColor == CHESS_COLOR_WHITE)
                        pCell = pDeck->GetCell(9 - number, literNumber);
                    else
                        pCell = pDeck->GetCell(number, 9 - literNumber);

                    IChessman *pChessman = pCell->GetChessman();

                    if (cellLine == 1 && pChessman)
                    {
                        PrintChessmanCell(pChessman->GetChessmanValue(), pChessman->GetChessmanColor(),
                            pCell->GetCellColor());
                    }
                    else
                        PrintEmptyCell(pCell->GetCellColor());
                }
            }
        }
    }
}

void ConsoleUIHelper::PrintEmptyCell(int cellColor)
{
    char cFiller = (cellColor == CHESS_COLOR_WHITE ? WHITE_CHAR : ' ');
    std::cout << cFiller << cFiller << cFiller << cFiller << "|";
}

void ConsoleUIHelper::PrintBottomTreshold()
{
    std::cout << "____|";
}

void ConsoleUIHelper::PrintTheTopTreshold()
{
    for (int i = 0; i < 41; i++)
    {
        std::cout << "_";
    }
    std::cout << std::endl;
}

void ConsoleUIHelper::PrintChessmanCell(ChessmanValue figureValue, int figureColor, int cellColor)
{
    char cFiller = (cellColor == CHESS_COLOR_WHITE ? WHITE_CHAR : ' ');
    std::string chessmanName = GetFigureName(figureValue, figureColor);
    if (chessmanName.size() == 2)
        chessmanName.push_back(cFiller);

    std::cout << cFiller << chessmanName << "|";
}

std::string ConsoleUIHelper::GetFigureName(ChessmanValue figureValue, int figureColor)
{
    std::string res = (figureColor == CHESS_COLOR_WHITE ? "w" : "b");

    switch (figureValue)
    {
        case FigurePawn:
            res.append("P");
            break;
        case FigureElephant:
            res.append("E");
            break;
        case FigureKnight:
            res.append("K");
            break;
        case FigureBoat:
            res.append("B");
            break;
        case FigureQueen:
            res.append("Q");
            break;
        case FigureKing:
            res.append("Kg");
            break;
        default:
            break;
    }
    return res;
}

char ConsoleUIHelper::GetLiterByNumber(int number)
{
    return Liters[number - 1];
}

std::string ConsoleUIHelper::GetCellName(int number, int literNumber)
{
    std::string cellName;

    cellName += GetLiterByNumber(literNumber);
    cellName += (char)(number + '0');

    return cellName;
}