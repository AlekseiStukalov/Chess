#include "stdafx.h"
#include "ChessMFC.h"
#include "ChessMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void RedrawFlags::SetAll(bool bValue)
{
    bRedrawDesk = bValue;
    bRedrawMarks = bValue;
}

// CChessMFCDlg

CChessMFCDlg::CChessMFCDlg(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_CHESSMFC_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pDeckEngine = nullptr;
    m_CurrentPlayerColor = CHESS_COLOR_WHITE;

    //default
    m_nCellSize = 64;
    m_nCoordFieldWidth = 30;
}

void CChessMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChessMFCDlg, CDialog)
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_GETMINMAXINFO()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CChessMFCDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    ChessEngine engine;
    m_pDeckEngine = engine.CreateDeck();
    m_RedrawFlags.SetAll(true);

    m_bmChessmen.LoadBitmap(IDB_CHESSMEN);
    m_ilChessmen.Create(IDB_CHESSMEN, 64, 0, RGB(0, 0, 0));

    m_StartPoint.x = 10;
    m_StartPoint.y = 10;


    return TRUE;
}

void CChessMFCDlg::DrawCoordinates(CDC* pDC)
{
    int deckStartPosX = m_StartPoint.x + m_nCoordFieldWidth;
    int deckStartPosY = m_StartPoint.y + m_nCoordFieldWidth;

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
        wchar_t c = L'a' + literNumber;
        CString str(c);

        int left = deckStartPosX + literNumber*m_nCellSize;
        int right = left + m_nCellSize;
        CRect rcTop(left, m_StartPoint.y, right, deckStartPosY);
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

        CRect rcLeft(m_StartPoint.x, top, deckStartPosX, bottom);
        CRect rcRight(rLeft, top, rRight, bottom);

        pDC->DrawText(str, rcLeft, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->DrawText(str, rcRight, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    pDC->SelectObject(pOldFont);
    pDC->SetBkColor(oldColor);
}

void CChessMFCDlg::DrawRawDesk(CDC* pDC)
{
    int deckStartPosX = m_StartPoint.x + m_nCoordFieldWidth;
    int deckStartPosY = m_StartPoint.y + m_nCoordFieldWidth;

    for (int number = 0; number < 8; number++)
    {
        for (int literNumber = 0; literNumber < 8; literNumber++)
        {
            IDeckCell* pCell = m_pDeckEngine->GetCell(number + 1, literNumber + 1);

            COLORREF color;
            if (pCell->GetCellColor() == CHESS_COLOR_BLACK)
                color = RGB(150, 60, 0);
            else
                color = RGB(240, 230, 50);

            int left = deckStartPosX + literNumber*m_nCellSize;
            int right = left + m_nCellSize;

            int topPosAbs = number*m_nCellSize;
            topPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? m_nCellSize * 7 - topPosAbs : topPosAbs);

            int top = deckStartPosY + topPosAbs;
            int bottom = top + m_nCellSize;

            CRect rc(left, top, right, bottom);

            FillRect(*pDC, rc, CBrush(color));
        }
    }
}

void CChessMFCDlg::DrawDesk(CDC* pDC)
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

void CChessMFCDlg::DrawChessmen(CDC* pDC)
{
    int deckStartPosX = m_StartPoint.x + m_nCoordFieldWidth;
    int deckStartPosY = m_StartPoint.y + m_nCoordFieldWidth;

    for (int number = 0; number < 8; number++)
    {
        for (int literNumber = 0; literNumber < 8; literNumber++)
        {
            IDeckCell *pCell = m_pDeckEngine->GetCell(number + 1, literNumber + 1);
            IChessman *pChessman = pCell->GetChessman();

            if (pChessman)
            {
                int left = deckStartPosX + literNumber*m_nCellSize;
                int right = left + m_nCellSize;

                int topPosAbs = number*m_nCellSize;
                topPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? m_nCellSize * 7 - topPosAbs : topPosAbs);

                int top = deckStartPosY + topPosAbs;
                int bottom = top + m_nCellSize;

                CRect rc(left, top, right, bottom);
                DrawChessman(pDC, pChessman->GetChessmanValue(), pChessman->GetChessmanColor(), rc);
            }
        }
    }
}

void CChessMFCDlg::DrawMarks(CDC* pDC)
{
    DrawMark(pDC, m_StepsPossibility.CanStep, RGB(0, 255, 0));
    DrawMark(pDC, m_StepsPossibility.CanKill, RGB(255, 0, 0));
    DrawMark(pDC, m_StepsPossibility.CanProtect, RGB(0, 0, 255));
}

void CChessMFCDlg::DrawMark(CDC* pDC, std::vector<CellPos>& positions, COLORREF color)
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

BOOL CChessMFCDlg::OnEraseBkgnd(CDC* pDC)
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
        int borderOffsetX = m_StartPoint.x - 1;
        int borderOffsetY = m_StartPoint.y - 1;
        int deckWdth = m_nCellSize * 8 + m_nCoordFieldWidth * 2;
        pDC->MoveTo(borderOffsetX, borderOffsetY);
        pDC->LineTo(borderOffsetX + deckWdth, borderOffsetY);
        pDC->LineTo(borderOffsetX + deckWdth, borderOffsetY + deckWdth);
        pDC->LineTo(borderOffsetX, borderOffsetY + deckWdth);
        pDC->LineTo(borderOffsetX, borderOffsetY);
    }

    DrawDesk(pDC);

    if(pOldPen)
        pDC->SelectObject(pOldPen);


    return TRUE;
}

void CChessMFCDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

HCURSOR CChessMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CChessMFCDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CDialog::OnGetMinMaxInfo(lpMMI);

    CRect rcC, rcW;
    GetWindowRect(&rcW);
    GetClientRect(&rcC);
    rcW.DeflateRect(rcC);

    lpMMI->ptMinTrackSize.x = rcW.Width();
    lpMMI->ptMinTrackSize.y = rcW.Height();

    lpMMI->ptMinTrackSize.x += m_StartPoint.x * 2;
    lpMMI->ptMinTrackSize.y += m_StartPoint.y * 2;

    lpMMI->ptMinTrackSize.x += 2 * m_nCoordFieldWidth + 8 * m_nCellSize;
    lpMMI->ptMinTrackSize.y += 2 * m_nCoordFieldWidth + 8 * m_nCellSize;
}

void CChessMFCDlg::DrawChessman(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect)
{
    int imgNumber = GetBmChessmanNumber(value, color);
    m_ilChessmen.Draw(pDC, imgNumber, CPoint(rect.left, rect.top), ILD_TRANSPARENT);
}

int CChessMFCDlg::GetBmChessmanNumber(ChessmanValue value, ChessColor color)
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

CRect CChessMFCDlg::GetRectByCellPos(CellPos &pos)
{
    int deckStartPosX = m_StartPoint.x + m_nCoordFieldWidth;
    int deckStartPosY = m_StartPoint.y + m_nCoordFieldWidth;

    CRect rect;
    rect.left = deckStartPosX + (pos.LiterNumber - 1)*m_nCellSize;
    rect.right = rect.left + m_nCellSize;

    int topPosAbs = (pos.Number - 1)*m_nCellSize;
    topPosAbs = (m_CurrentPlayerColor == CHESS_COLOR_WHITE ? m_nCellSize * 7 - topPosAbs : topPosAbs);

    rect.top = deckStartPosY + topPosAbs;
    rect.bottom = rect.top + m_nCellSize;

    return rect;
}

HBITMAP CreateBitmapMask(HBITMAP hbmColour)
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

void CChessMFCDlg::DrawChessman__Resizeable(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect)
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

int CChessMFCDlg::GetBmChessmanOffset(ChessmanValue value, ChessColor color)
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

void CChessMFCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);

    CPoint pos;
    ::GetCursorPos(&pos);
    ScreenToClient(&pos);

    pos.x -= (m_StartPoint.x + m_nCoordFieldWidth);
    pos.y -= (m_StartPoint.y + m_nCoordFieldWidth);

    m_RedrawFlags.SetAll(false);

    int deckWidth = m_nCellSize * 8;
    if (pos.x >= 0 && pos.y >= 0 && pos.x < deckWidth && pos.y < deckWidth)
    {
        CellPos cellPos;
        cellPos.LiterNumber = (pos.x / m_nCellSize) + 1;

        if(m_CurrentPlayerColor == CHESS_COLOR_WHITE)
            cellPos.Number = 8 - (int)(pos.y / m_nCellSize);
        else
            cellPos.Number = (int)(pos.y / m_nCellSize) + 1;

        bool wasStep = false;

        if (m_StepsPossibility.Empty())
            m_RedrawFlags.bRedrawMarks = true;
        else
        {
            StepResult res;

            m_RedrawFlags.SetAll(true);

            IDeckCell* pCurrCell = m_pDeckEngine->GetCell(m_SelectedCell);
            IDeckCell* pNewCell = m_pDeckEngine->GetCell(cellPos);
            if (pNewCell && pCurrCell)
            {
                if (m_StepsPossibility.IsPosExists(m_StepsPossibility.CanKill, cellPos) ||
                    m_StepsPossibility.IsPosExists(m_StepsPossibility.CanStep, cellPos))
                {
                    res = m_pDeckEngine->MakeStep(m_CurrentPlayerColor, pCurrCell->GetCellName(), pNewCell->GetCellName());
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
            m_StepsPossibility = m_pDeckEngine->GetPossibleSteps(cellPos);
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

void CChessMFCDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    m_RedrawFlags.SetAll(true);

    m_nCellSize = 64;
    m_nCoordFieldWidth = 30;
}
