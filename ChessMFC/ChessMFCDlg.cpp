#include "stdafx.h"
#include "ChessMFC.h"
#include "ChessMFCDlg.h"
#include "afxdialogex.h"
#include "TwoPlayersDlg.h"

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
    m_nActiveDlg = IDD_CHESS_MENU;

    m_ViewIds.push_back(IDD_CHESS_MENU);
    m_ViewIds.push_back(IDD_TWO_PLAYERS);
    m_ViewIds.push_back(IDD_SOLVE_PROBLEM);
}

void CChessMFCDlg::SetNewDlg(int dialogId)
{
    m_nActiveDlg = dialogId;
    UpdateLayout();
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

    m_TwoPlayersDlg.SetEngine(m_pDeckEngine);
    m_TwoPlayersDlg.Create(IDD_TWO_PLAYERS, this);
    m_TwoPlayersDlg.SetCurrentPlayerColor(m_CurrentPlayerColor);

    m_MenuDlg.Create(IDD_CHESS_MENU, this);
    m_SolveProblemDlg.Create(IDD_SOLVE_PROBLEM, this);

    m_StartPoint.x = m_StartPoint.y = 10;

    UpdateLayout();

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

    switch (m_nActiveDlg)
    {
        case IDD_CHESS_MENU:
        {
            lpMMI->ptMinTrackSize.x += 440;
            lpMMI->ptMinTrackSize.y += 250;
            break;
        }
        case IDD_SOLVE_PROBLEM:
        case IDD_TWO_PLAYERS:
        {
            lpMMI->ptMinTrackSize.x += m_StartPoint.x * 2;
            lpMMI->ptMinTrackSize.y += m_StartPoint.y * 2;

            if (m_TwoPlayersDlg.GetSafeHwnd())
            {
                CSize size = m_TwoPlayersDlg.GetRequiredSize();
                lpMMI->ptMinTrackSize.x += size.cx;
                lpMMI->ptMinTrackSize.y += size.cy;
            }
            else
            {
                lpMMI->ptMinTrackSize.x += DEFAULT_DECK_SIZE;
                lpMMI->ptMinTrackSize.y += DEFAULT_DECK_SIZE;
            }
            break;
        }
        default:
            break;
    }
}

void CChessMFCDlg::UpdateLayout()
{
    CRect wndRect;
    GetWindowRect(&wndRect);

    for each (int dlgId in m_ViewIds)
        GetDlgById(dlgId)->ShowWindow(SW_HIDE);

    switch (m_nActiveDlg)
    {
        case IDD_CHESS_MENU:
        {
            MoveWindow(CRect(wndRect.left, wndRect.top, 100, 100));
            m_MenuDlg.ShowWindow(SW_SHOW);
            break;
        }
        case IDD_TWO_PLAYERS:
        {
            m_pDeckEngine->PopulateDeckForGame();
            MoveWindow(CRect(wndRect.left, wndRect.top, 100, 100));
            m_TwoPlayersDlg.ShowWindow(SW_SHOW);
            break;
        }
        case IDD_SOLVE_PROBLEM:
        {
            MoveWindow(CRect(wndRect.left, wndRect.top, 100, 100));
            m_SolveProblemDlg.ShowWindow(SW_SHOW);
            break;
        }
        default:
            break;
    }
}

CDialog* CChessMFCDlg::GetDlgById(int dialogId)
{
    switch (dialogId)
    {
        case IDD_CHESS_MENU:
            return &m_MenuDlg;
        case IDD_TWO_PLAYERS:
            return &m_TwoPlayersDlg;
        case IDD_SOLVE_PROBLEM:
            return &m_SolveProblemDlg;
        default:
            return nullptr;
    }
}

void CChessMFCDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    switch (m_nActiveDlg)
    {
        case IDD_CHESS_MENU:
        {
            if (m_MenuDlg.GetSafeHwnd())
            {
                CRect rc;
                GetClientRect(&rc);
                m_MenuDlg.MoveWindow(0, 0, rc.Width(), rc.Height());
            }
            break;
        }
        case IDD_TWO_PLAYERS:
        {
            if (m_TwoPlayersDlg.GetSafeHwnd())
            {
                CSize size = m_TwoPlayersDlg.GetRequiredSize();
                m_TwoPlayersDlg.MoveWindow(m_StartPoint.x, m_StartPoint.y, size.cx, size.cy);
            }
            break;
        }
        case IDD_SOLVE_PROBLEM:
        {
            if (m_SolveProblemDlg.GetSafeHwnd())
            {
                CSize size = m_SolveProblemDlg.GetRequiredSize();
                m_SolveProblemDlg.MoveWindow(m_StartPoint.x, m_StartPoint.y, size.cx, size.cy);
            }
            break;
        }
        default:
            break;
    }
}
