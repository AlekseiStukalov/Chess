
// ChessMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChessMFC.h"
#include "ChessMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChessMFCDlg dialog

CChessMFCDlg::CChessMFCDlg(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_CHESSMFC_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nCellSize = 0;
    m_nCoordFieldWidth = 0;
    m_pDeckEngine = nullptr;
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

    m_bmChessmen.LoadBitmap(IDB_CHESSMEN);

    m_StartPoint.x = 10;
    m_StartPoint.y = 10;


    return TRUE;
}

void CChessMFCDlg::DrawCoordinates(CDC* pDC, CPoint &lTop, ChessColor currentPlayerColor)
{
    COLORREF oldColor = pDC->SetBkColor(RGB(255, 255, 255));

    CFont* pOldFont = pDC->GetCurrentFont();
    LOGFONT logFont;
    pOldFont->GetLogFont(&logFont);
    logFont.lfWeight = FW_BOLD;
    logFont.lfHeight = 13;

    CFont newFont;
    newFont.CreateFontIndirect(&logFont);
    pDC->SelectObject(&newFont);

    int bTop = m_nCoordFieldWidth + m_nCellSize * 8 + lTop.y;
    int bBottom = bTop + m_nCoordFieldWidth;
    for (int literNumber = 0; literNumber < 8; literNumber++)
    {
        wchar_t c = L'a' + literNumber;
        CString str(c);

        int left = m_nCoordFieldWidth + literNumber*m_nCellSize + lTop.x;
        int right = left + m_nCellSize;
        CRect rcTop(left, lTop.y, right, m_nCoordFieldWidth + lTop.y);
        CRect rcBottom(left, bTop, right, bBottom);

        pDC->DrawText(str, rcTop, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->DrawText(str, rcBottom, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    int rLeft = m_nCoordFieldWidth + m_nCellSize * 8 + lTop.x;
    int rRight = rLeft + m_nCoordFieldWidth;
    for (int i = 0; i < 8; i++)
    {
        wchar_t c = L'8' - i;
        CString str(c);

        int top = m_nCoordFieldWidth + i*m_nCellSize + lTop.y;
        int bottom = top + m_nCellSize;

        CRect rcLeft(lTop.x, top, m_nCoordFieldWidth + lTop.x, bottom);
        CRect rcRight(rLeft, top, rRight, bottom);

        pDC->DrawText(str, rcLeft, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->DrawText(str, rcRight, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    pDC->SelectObject(pOldFont);
    pDC->SetBkColor(oldColor);
}

void CChessMFCDlg::DrawDesk(CDC* pDC, CPoint &lTop, ChessColor currentPlayerColor)
{
    DrawCoordinates(pDC, lTop, currentPlayerColor);

    int deckStartPosX = lTop.x + m_nCoordFieldWidth;
    int deckStartPosY = lTop.y + m_nCoordFieldWidth;

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

            int left = literNumber*m_nCellSize + deckStartPosX;
            int right = left + m_nCellSize;

            int top = m_nCellSize * 7 - number*m_nCellSize + deckStartPosY;
            int bottom = top + m_nCellSize;

            CRect rc(left, top, right, bottom);

            FillRect(*pDC, rc, CBrush(color));
        }
    }

    DrawChessmen(pDC, lTop, currentPlayerColor);
}

void CChessMFCDlg::DrawChessmen(CDC* pDC, CPoint &lTop, ChessColor currentPlayerColor)
{
    int deckStartPosX = lTop.x + m_nCoordFieldWidth;
    int deckStartPosY = lTop.y + m_nCoordFieldWidth;

    for (int number = 0; number < 8; number++)
    {
        for (int literNumber = 0; literNumber < 8; literNumber++)
        {
            IDeckCell *pCell = m_pDeckEngine->GetCell(number + 1, literNumber + 1);
            IChessman *pChessman = pCell->GetChessman();

            if (pChessman)
            {
                int left = literNumber*m_nCellSize + deckStartPosX;
                int right = left + m_nCellSize;

                int top = m_nCellSize * 7 - number*m_nCellSize + deckStartPosY;
                int bottom = top + m_nCellSize;

                CRect rc(left, top, right, bottom);
                DrawChessman(pDC, pChessman->GetChessmanValue(), pChessman->GetChessmanColor(), rc);
            }
        }
    }
}

BOOL CChessMFCDlg::OnEraseBkgnd(CDC* pDC)
{
    CDialog::OnEraseBkgnd(pDC);

    CRect cRect;
    GetClientRect(cRect);
    FillRect(*pDC, cRect, CBrush(RGB(255, 255, 255)));

    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen *pOldPen = pDC->SelectObject(&pen);

    int offset = 10;
    int borderOffset = offset - 1;


    int deckWdth = m_nCellSize * 8 + m_nCoordFieldWidth * 2;
    pDC->MoveTo(CPoint(borderOffset, borderOffset));
    pDC->LineTo(CPoint(borderOffset + deckWdth, borderOffset));
    pDC->LineTo(CPoint(borderOffset + deckWdth, borderOffset + deckWdth));
    pDC->LineTo(CPoint(borderOffset, borderOffset + deckWdth));
    pDC->LineTo(CPoint(borderOffset, borderOffset));

    DrawDesk(pDC, CPoint(offset, offset), CHESS_COLOR_WHITE);
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

        CPaintDC dc(this);

        //DrawDesk(&dc, CPoint(0, 0), CPoint(500, 500));
        //DrawChessmen(&dc, m_StartPoint, CHESS_COLOR_WHITE);

    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChessMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CChessMFCDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 600;
    lpMMI->ptMinTrackSize.y = 630;
}

HRESULT CChessMFCDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
    OnOK();
    return S_OK;
}

HRESULT CChessMFCDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
    OnCancel();
    return S_OK;
}

void CChessMFCDlg::DrawChessman(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect)
{
    CImageList imgList;
    imgList.Create(IDB_CHESSMEN, 64, 0, RGB(0, 0, 0));
    int imgNumber = GetBmChessmanNumber(value, color);
    imgList.Draw(pDC, imgNumber, CPoint(rect.left, rect.top), ILD_TRANSPARENT);
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





    int a = 0;
    a++;

}

void CChessMFCDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    m_nCellSize = 64;
    m_nCoordFieldWidth = 30;
}
