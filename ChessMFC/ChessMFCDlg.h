#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "ChessMenuDlg.h"
#include "TwoPlayersDlg.h"
#include "SolveProblemDlg.h"

class IDeckEngine;

class CChessMFCDlg : public CDialog
{
public:
    CChessMFCDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHESSMFC_DIALOG, IDH = IDR_HTML_CHESSMFC_DIALOG };
#endif

    void SetNewDlg(int dialogId);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

    void UpdateLayout();
    CDialog* GetDlgById(int dialogId);

    DECLARE_MESSAGE_MAP()

private:
    int m_nActiveDlg;
    std::vector<int> m_ViewIds;

    CTwoPlayersDlg m_TwoPlayersDlg;
    CChessMenu m_MenuDlg;
    CSolveProblemDlg m_SolveProblemDlg;

    HICON m_hIcon;
    CPoint m_StartPoint;

    IDeckEngine *m_pDeckEngine;

    ChessColor m_CurrentPlayerColor;
    CellPos m_SelectedCell;
};
