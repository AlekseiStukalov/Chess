#include "stdafx.h"
#include "DeckDrawer.h"


DeckDrawer::DeckDrawer()
{
    m_CurrentPlayerColor = CHESS_COLOR_WHITE;
    m_nCellSize = 0;
    m_nCoordFieldWidth = 0;
    m_nBorderWidth = 0;
}

DeckDrawer::~DeckDrawer()
{
}

void DeckDrawer::SetDeckParameters(int cellSize, int coordFieldWidth, int borderWidth)
{
    m_nCellSize = cellSize;
    m_nCoordFieldWidth = coordFieldWidth;
    m_nBorderWidth = borderWidth;
}

void DeckDrawer::SetCurrentPlayerColor(ChessColor color)
{
    m_CurrentPlayerColor = color;
}

CSize DeckDrawer::GetRequiredSize()
{
    CSize size;
    size.cx = 8 * m_nCellSize + 2 * (m_nCoordFieldWidth + m_nBorderWidth);
    size.cy = size.cx;
    return size;
}

void DeckDrawer::DrawBorder(CDC *pDC, CRect &clientRect)
{
    CBrush whiteBrush(RGB(255, 255, 255));
    pDC->FillRect(clientRect, &whiteBrush);

    CPen pen(PS_SOLID, m_nBorderWidth, RGB(0, 0, 0));
    CPen *pOldPen = pDC->SelectObject(&pen);
    int lastPixel = GetRequiredSize().cx - 1;
    pDC->MoveTo(0, 0);
    pDC->LineTo(lastPixel, 0);
    pDC->LineTo(lastPixel, lastPixel);
    pDC->LineTo(0, lastPixel);
    pDC->LineTo(0, 0);

    pDC->SelectObject(pOldPen);
}

void DeckDrawer::DrawCoordinates(CDC* pDC)
{
    int deckStartPosX = m_nCoordFieldWidth;
    int deckStartPosY = m_nCoordFieldWidth;

    COLORREF oldColor = pDC->SetBkColor(RGB(255, 255, 255));

    CFont* pOldFont = pDC->GetCurrentFont();
    LOGFONT logFont;
    pOldFont->GetLogFont(&logFont);
    logFont.lfWeight = FW_BOLD;
    logFont.lfHeight = 13;

    CFont newFont;
    newFont.CreateFontIndirect(&logFont);
    pDC->SelectObject(&newFont);

    //draw liters
    int bTop = deckStartPosY + m_nCellSize * 8;
    int bBottom = bTop + m_nCoordFieldWidth;
    for (int literNumber = 0; literNumber < 8; literNumber++)
    {
        wchar_t c = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? (L'a' + literNumber) : (L'h' - literNumber));
        CString str(c);

        int left = deckStartPosX + literNumber*m_nCellSize;
        int right = left + m_nCellSize;
        CRect rcTop(left, /*m_StartPoint.y*/0, right, deckStartPosY);
        CRect rcBottom(left, bTop, right, bBottom);

        pDC->DrawText(str, rcTop, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->DrawText(str, rcBottom, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    //draw numbers
    int rLeft = deckStartPosX + m_nCellSize * 8;
    int rRight = rLeft + m_nCoordFieldWidth;
    for (int i = 0; i < 8; i++)
    {
        wchar_t c = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? (L'8' - i) : (L'1' + i));
        CString str(c);

        int top = deckStartPosY + i*m_nCellSize;
        int bottom = top + m_nCellSize;

        CRect rcLeft(/*m_StartPoint.x*/0, top, deckStartPosX, bottom);
        CRect rcRight(rLeft, top, rRight, bottom);

        pDC->DrawText(str, rcLeft, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->DrawText(str, rcRight, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    pDC->SelectObject(pOldFont);
    pDC->SetBkColor(oldColor);
}

void DeckDrawer::DrawRawDesk(CDC* pDC)
{
    int deckStartPosX = m_nCoordFieldWidth + m_nBorderWidth;
    int deckStartPosY = m_nCoordFieldWidth + m_nBorderWidth;

    //A1 is BLACK
    ChessColor currentCellColor = CHESS_COLOR_BLACK;
    for (int number = 1; number <= 8; number++)
    {
        for (int liter = 'A'; liter <= 'H'; liter++)
        {
            int literNumber = liter - 'A';

            COLORREF color;
            if (currentCellColor == CHESS_COLOR_BLACK)
                color = RGB(150, 60, 0);
            else
                color = RGB(240, 230, 50);

            int left = deckStartPosX + literNumber*m_nCellSize;
            int right = left + m_nCellSize;

            int top = deckStartPosY + m_nCellSize * (8 - number);
            int bottom = top + m_nCellSize;

            CRect rc(left, top, right, bottom);

            FillRect(*pDC, rc, CBrush(color));

            currentCellColor = (currentCellColor == CHESS_COLOR_WHITE ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE);
        }

        currentCellColor = (currentCellColor == CHESS_COLOR_WHITE ? CHESS_COLOR_BLACK : CHESS_COLOR_WHITE);
    }
}

