#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "DeckDialog.h"

class IDeckEngine;


class CChessMFCDlg : public CDialog
{
public:
    CChessMFCDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHESSMFC_DIALOG, IDH = IDR_HTML_CHESSMFC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

protected:


    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

    DECLARE_MESSAGE_MAP()

private:
    CDeckDialog m_DeckDlg;
    HICON m_hIcon;


    CPoint m_StartPoint;
    CellPos m_SelectedCell;

    IDeckEngine *m_pDeckEngine;
    ChessColor m_CurrentPlayerColor;
};
