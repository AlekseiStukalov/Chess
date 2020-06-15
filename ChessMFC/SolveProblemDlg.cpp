#include "stdafx.h"
#include "ChessMFC.h"
#include "SolveProblemDlg.h"


IMPLEMENT_DYNAMIC(CSolveProblemDlg, CDialog)

CSolveProblemDlg::CSolveProblemDlg(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_SOLVE_PROBLEM, pParent)
{
    m_nCellSize = 64;
    m_nCoordFieldWidth = 30;
    m_nBorderWidth = 1;
    m_CurrentPlayerColor = CHESS_COLOR_WHITE;
}

CSolveProblemDlg::~CSolveProblemDlg()
{}

BEGIN_MESSAGE_MAP(CSolveProblemDlg, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CSolveProblemDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_DeckDrawer.SetDeckParameters(m_nCellSize, m_nCoordFieldWidth, m_nBorderWidth);
    m_DeckDrawer.SetCurrentPlayerColor(m_CurrentPlayerColor);
    return TRUE;
}

void CSolveProblemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDown(nFlags, point);

}

BOOL CSolveProblemDlg::OnEraseBkgnd(CDC* pDC)
{
    CDialog::OnEraseBkgnd(pDC);
    DrawDesk(pDC);
    return TRUE;
}

void CSolveProblemDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

}

CSize CSolveProblemDlg::GetRequiredSize()
{
    return m_DeckDrawer.GetRequiredSize();
}

void CSolveProblemDlg::DrawDesk(CDC* pDC)
{
    CRect cRect;
    GetClientRect(cRect);

    m_DeckDrawer.DrawBorder(pDC, cRect);
    m_DeckDrawer.DrawCoordinates(pDC);
    m_DeckDrawer.DrawRawDesk(pDC);
}



