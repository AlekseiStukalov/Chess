#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "ChessMenu.h"
#include "DeckDialog.h"

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

    DECLARE_MESSAGE_MAP()

private:
    int m_nActiveDlg;
    CDeckDialog m_DeckDlg;
    CChessMenu m_MenuDlg;

    HICON m_hIcon;
    CPoint m_StartPoint;
    CellPos m_SelectedCell;

    IDeckEngine *m_pDeckEngine;
    ChessColor m_CurrentPlayerColor;
};
