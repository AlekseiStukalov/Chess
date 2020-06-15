#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "DeckDrawer.h"


class CSolveProblemDlg : public CDialog
{
    DECLARE_DYNAMIC(CSolveProblemDlg)
    DECLARE_MESSAGE_MAP()

public:
    CSolveProblemDlg(CWnd* pParent = NULL);
    virtual ~CSolveProblemDlg();
    virtual BOOL OnInitDialog();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CSize GetRequiredSize();

protected:
    void DrawDesk(CDC* pDC);

    DeckDrawer m_DeckDrawer;
    int m_nCellSize;
    int m_nCoordFieldWidth;
    int m_nBorderWidth;
    ChessColor m_CurrentPlayerColor;

};


