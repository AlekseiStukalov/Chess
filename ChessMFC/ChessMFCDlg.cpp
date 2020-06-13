#include "stdafx.h"
#include "ChessMFC.h"
#include "ChessMFCDlg.h"
#include "afxdialogex.h"
#include "DeckDialog.h"

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
END_MESSAGE_MAP()


BOOL CChessMFCDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    ChessEngine engine;
    m_pDeckEngine = engine.CreateDeck();

    m_DeckDlg.SetEngine(m_pDeckEngine);
    m_DeckDlg.Create(IDD_DECK, this);

    m_StartPoint.x = m_StartPoint.y = 0;

    return TRUE;
}

BOOL CChessMFCDlg::OnEraseBkgnd(CDC* pDC)
{
    CDialog::OnEraseBkgnd(pDC);
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

    if (m_DeckDlg.GetSafeHwnd())
    {
        CSize size = m_DeckDlg.GetRequiredSize();
        lpMMI->ptMinTrackSize.x += size.cx;
        lpMMI->ptMinTrackSize.y += size.cy;
    }
    else
    {
        lpMMI->ptMinTrackSize.x += DEFAULT_DECK_SIZE;
        lpMMI->ptMinTrackSize.y += DEFAULT_DECK_SIZE;
    }
}

void CChessMFCDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_DeckDlg.GetSafeHwnd())
    {
        CSize size = m_DeckDlg.GetRequiredSize();
        m_DeckDlg.MoveWindow(m_StartPoint.x, m_StartPoint.y, size.cx, size.cy);
    }
}
