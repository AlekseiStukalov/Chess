#include "stdafx.h"
#include "ChessMFC.h"
#include "DeckDialog.h"

IMPLEMENT_DYNAMIC(CDeckDialog, CDialog)

BEGIN_MESSAGE_MAP(CDeckDialog, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CDeckDialog::CDeckDialog(CWnd * pParent) : CDialog(IDD_DECK, pParent)
{
    //default
    m_nCellSize = 64;
    m_nCoordFieldWidth = 30;
    m_nBorderWidth = 1;
}

CDeckDialog::~CDeckDialog()
{
}

void CDeckDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);

    CPoint pos;
    ::GetCursorPos(&pos);
    ScreenToClient(&pos);

    pos.x -= (m_nBorderWidth + m_nCoordFieldWidth);
    pos.y -= (m_nBorderWidth + m_nCoordFieldWidth);

    m_RedrawFlags.SetAll(false);

    int deckWidth = m_nCellSize * 8;
    if (pos.x >= 0 && pos.y >= 0 && pos.x < deckWidth && pos.y < deckWidth)
    {
        CellPos cellPos;
        if (m_CurrentPlayerColor == CHESS_COLOR_WHITE)
        {
            cellPos.LiterNumber = (pos.x / m_nCellSize) + 1;
            cellPos.Number = 8 - (int)(pos.y / m_nCellSize);
        }
        else
        {
            cellPos.LiterNumber = 8 - (pos.x / m_nCellSize);
            cellPos.Number = (int)(pos.y / m_nCellSize) + 1;
        }

        bool wasStep = false;

        if (m_StepsPossibility.Empty())
            m_RedrawFlags.bRedrawMarks = true;
        else
        {
            StepResult res;

            m_RedrawFlags.SetAll(true);

            IDeckCell* pCurrCell = m_pEngine->GetCell(m_SelectedCell);
            IDeckCell* pNewCell = m_pEngine->GetCell(cellPos);
            if (pNewCell && pCurrCell)
            {
                if (m_StepsPossibility.IsPosExists(m_StepsPossibility.CanKill, cellPos) ||
                    m_StepsPossibility.IsPosExists(m_StepsPossibility.CanStep, cellPos))
                {
                    res = m_pEngine->MakeStep(m_CurrentPlayerColor, pCurrCell->GetCellName(), pNewCell->GetCellName());
                    if (res.nError == E_NO_ERROR)
                    {
                        wasStep = true;
                        m_CurrentPlayerColor = res.nNextPlayerColor;
                    }
                }
            }
        }

        if (wasStep)
        {
            m_StepsPossibility.Clear();
            m_SelectedCell.LiterNumber = 0;
            m_SelectedCell.Number = 0;
        }
        else
        {
            m_StepsPossibility = m_pEngine->GetPossibleSteps(cellPos);
            m_SelectedCell = cellPos;
        }
    }
    else
    {
        if (!m_StepsPossibility.Empty())
        {
            m_StepsPossibility.Clear();
            m_RedrawFlags.SetAll(true);
        }

        m_SelectedCell.LiterNumber = 0;
        m_SelectedCell.Number = 0;
    }

    RedrawWindow();
}

void CDeckDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    m_RedrawFlags.SetAll(true);

    m_nCellSize = 64;
    m_nCoordFieldWidth = 30;
}

void CDeckDialog::SetEngine(IDeckEngine *pDeckEngine)
{
    m_pEngine = pDeckEngine;
}

void CDeckDialog::SetCurrentPlayerColor(ChessColor currentPlayerColor)
{
    m_CurrentPlayerColor = currentPlayerColor;
}

CSize CDeckDialog::GetRequiredSize()
{
    CSize size;
    size.cx = 8 * m_nCellSize + 2 * (m_nCoordFieldWidth + m_nBorderWidth);
    size.cy = size.cx;
    return size;
}

BOOL CDeckDialog::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_RedrawFlags.SetAll(true);

    m_bmChessmen.LoadBitmap(IDB_CHESSMEN);
    m_ilChessmen.Create(IDB_CHESSMEN, 64, 0, RGB(0, 0, 0));

    return TRUE;
}

BOOL CDeckDialog::OnEraseBkgnd(CDC* pDC)
{
    CPen *pOldPen = nullptr;
    if (m_RedrawFlags.bRedrawDesk)
    {
        CDialog::OnEraseBkgnd(pDC);
        CRect cRect;
        GetClientRect(cRect);
        FillRect(*pDC, cRect, CBrush(RGB(255, 255, 255)));

        CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
        CPen *pOldPen = pDC->SelectObject(&pen);
        int lastPixel = GetRequiredSize().cx - 1;
        pDC->MoveTo(0, 0);
        pDC->LineTo(lastPixel, 0);
        pDC->LineTo(lastPixel, lastPixel);
        pDC->LineTo(0, lastPixel);
        pDC->LineTo(0, 0);
    }

    DrawDesk(pDC);

    if (pOldPen)
        pDC->SelectObject(pOldPen);

    return TRUE;
}

void CDeckDialog::DrawCoordinates(CDC* pDC)
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

void CDeckDialog::DrawRawDesk(CDC* pDC)
{
    int deckStartPosX = m_nCoordFieldWidth + m_nBorderWidth;
    int deckStartPosY = m_nCoordFieldWidth + m_nBorderWidth;

    for (int number = 0; number < 8; number++)
    {
        for (int literNumber = 0; literNumber < 8; literNumber++)
        {
            IDeckCell* pCell = m_pEngine->GetCell(number + 1, literNumber + 1);

            COLORREF color;
            if (pCell->GetCellColor() == CHESS_COLOR_BLACK)
                color = RGB(150, 60, 0);
            else
                color = RGB(240, 230, 50);

            int left = deckStartPosX + literNumber*m_nCellSize;
            int right = left + m_nCellSize;

            int top = deckStartPosY + m_nCellSize * 7 - number*m_nCellSize;
            int bottom = top + m_nCellSize;

            CRect rc(left, top, right, bottom);

            FillRect(*pDC, rc, CBrush(color));
        }
    }
}

void CDeckDialog::DrawDesk(CDC* pDC)
{
    if (m_RedrawFlags.bRedrawDesk)
    {
        DrawCoordinates(pDC);
        DrawRawDesk(pDC);
        DrawChessmen(pDC);
    }

    if (m_RedrawFlags.bRedrawMarks)
        DrawMarks(pDC);
}

void CDeckDialog::DrawChessmen(CDC* pDC)
{
    int deckStartPosX = m_nCoordFieldWidth + m_nBorderWidth;
    int deckStartPosY = m_nCoordFieldWidth + m_nBorderWidth;

    for (int number = 0; number < 8; number++)
    {
        for (int literNumber = 0; literNumber < 8; literNumber++)
        {
            IDeckCell *pCell = m_pEngine->GetCell(number + 1, literNumber + 1);
            IChessman *pChessman = pCell->GetChessman();

            if (pChessman)
            {
                int horPosAbs = literNumber*m_nCellSize;
                horPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? horPosAbs : m_nCellSize * 7 - horPosAbs);

                int left = deckStartPosX + horPosAbs;
                int right = left + m_nCellSize;

                int vertPosAbs = number*m_nCellSize;
                vertPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? m_nCellSize * 7 - vertPosAbs : vertPosAbs);

                int top = deckStartPosY + vertPosAbs;
                int bottom = top + m_nCellSize;

                CRect rc(left, top, right, bottom);
                DrawChessman(pDC, pChessman->GetChessmanValue(), pChessman->GetChessmanColor(), rc);
            }
        }
    }
}

void CDeckDialog::DrawChessman__Resizeable(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect)
{
    //NOT IMPLEMENTED


    int bmOffset = GetBmChessmanOffset(value, color);


    CDC MemDC;
    MemDC.CreateCompatibleDC(pDC);

    MemDC.SelectObject(CreateBitmapMask((HBITMAP)m_bmChessmen.GetSafeHandle()));
    BitBlt(*pDC, 0, 0, 768, 64, MemDC, 0, 0, SRCAND);

    CBitmap* pOldBitmap = MemDC.SelectObject(&m_bmChessmen);
    BitBlt(*pDC, 0, 64, 768, 64, MemDC, 0, 0, SRCPAINT);


    pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC,
        bmOffset, 0, m_nCellSize, m_nCellSize, SRCCOPY);

    MemDC.SelectObject(pOldBitmap);
    MemDC.DeleteDC();


}

void CDeckDialog::DrawChessman(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect)
{
    int imgNumber = GetBmChessmanNumber(value, color);
    m_ilChessmen.Draw(pDC, imgNumber, CPoint(rect.left, rect.top), ILD_TRANSPARENT);
}

void CDeckDialog::DrawMarks(CDC* pDC)
{
    DrawMark(pDC, m_StepsPossibility.CanStep, RGB(0, 255, 0));
    DrawMark(pDC, m_StepsPossibility.CanKill, RGB(255, 0, 0));
    DrawMark(pDC, m_StepsPossibility.CanProtect, RGB(0, 0, 255));
    DrawMark(pDC, m_StepsPossibility.AdditionalCells, RGB(255, 0, 255));
}

void CDeckDialog::DrawMark(CDC* pDC, std::vector<CellPos>& positions, COLORREF color)
{
    CPen pen(PS_SOLID, 0, color);
    CPen *pOldPen = pDC->SelectObject(&pen);
    CBrush brush(color);
    CBrush *pOldBrush = pDC->SelectObject(&brush);
    for (size_t i = 0; i < positions.size(); i++)
    {
        CRect outerRc = GetRectByCellPos(positions[i]);
        CRect innerRc(outerRc);
        innerRc.InflateRect(-4, -4);

        pDC->Rectangle(outerRc.left, outerRc.top, outerRc.right, innerRc.top); //top
        pDC->Rectangle(outerRc.left, innerRc.bottom, outerRc.right, outerRc.bottom); //bottom
        pDC->Rectangle(innerRc.right, innerRc.top, outerRc.right, innerRc.bottom); //right
        pDC->Rectangle(outerRc.left, innerRc.top, innerRc.left, innerRc.bottom); //left
    }
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

int CDeckDialog::GetBmChessmanOffset(ChessmanValue value, ChessColor color)
{
    int offset = 0;
    if (color == CHESS_COLOR_BLACK)
        offset = 0x180;

    int pos = 0;

    switch (value)
    {
        case FigureBishop:
            pos = 0x040;
            break;
        case FigureKnight:
            pos = 0x100;
            break;
        case FigureRook:
            pos = 0;
            break;
        case FigureQueen:
            pos = 0x0C0;
            break;
        case FigureKing:
            pos = 0x080;
            break;
        case FigurePawn:
        default:
            pos = 0x140;
            break;
    }

    return pos + offset;
}

int CDeckDialog::GetBmChessmanNumber(ChessmanValue value, ChessColor color)
{
    int offset = 0;
    if (color == CHESS_COLOR_BLACK)
        offset = 6;

    int pos = 0;

    switch (value)
    {
        case FigureBishop:
            pos = 1;
            break;
        case FigureKnight:
            pos = 4;
            break;
        case FigureRook:
            pos = 0;
            break;
        case FigureQueen:
            pos = 3;
            break;
        case FigureKing:
            pos = 2;
            break;
        case FigurePawn:
        default:
            pos = 5;
            break;
    }

    return pos + offset;
}

CRect CDeckDialog::GetRectByCellPos(CellPos &pos)
{
    int deckStartPosX = m_nCoordFieldWidth + m_nBorderWidth;
    int deckStartPosY = m_nCoordFieldWidth + m_nBorderWidth;

    int horPosAbs = (pos.LiterNumber - 1)*m_nCellSize;
    horPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? horPosAbs : m_nCellSize * 7 - horPosAbs);

    int vertPosAbs = (pos.Number - 1)*m_nCellSize;
    vertPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? m_nCellSize * 7 - vertPosAbs : vertPosAbs);

    CRect rect;
    rect.left = deckStartPosX + horPosAbs;
    rect.right = rect.left + m_nCellSize;
    rect.top = deckStartPosY + vertPosAbs;
    rect.bottom = rect.top + m_nCellSize;

    return rect;
}

HBITMAP CDeckDialog::CreateBitmapMask(HBITMAP hbmColour)
{
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.  

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

    hdcMem = CreateCompatibleDC(0);
    hdcMem2 = CreateCompatibleDC(0);

    SelectObject(hdcMem, (HGDIOBJ)hbmColour);
    SelectObject(hdcMem2, (HGDIOBJ)hbmMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkMode(hdcMem, TRANSPARENT);
    //SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}
