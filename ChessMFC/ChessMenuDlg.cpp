#include "stdafx.h"
#include "ChessMFC.h"
#include "ChessMenuDlg.h"
#include "afxdialogex.h"
#include "ChessMFCDlg.h"

IMPLEMENT_DYNAMIC(CChessMenu, CDialog)

BEGIN_MESSAGE_MAP(CChessMenu, CDialog)
    ON_BN_CLICKED(IDC_BTN_2_PLAYERS, OnStart2Players)
    ON_BN_CLICKED(IDC_BTN_SOLVE_PROBLEM, OnStartProblemSolving)
END_MESSAGE_MAP()

CChessMenu::CChessMenu(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_CHESS_MENU, pParent)
{

}

CChessMenu::~CChessMenu()
{
}

void CChessMenu::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

void CChessMenu::OnStart2Players()
{
    ((CChessMFCDlg*)GetParent())->SetNewDlg(IDD_DECK);
}

void CChessMenu::OnStartProblemSolving()
{
    ((CChessMFCDlg*)GetParent())->SetNewDlg(IDD_SOLVE_PROBLEM);
}

