#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"

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

    void DrawCoordinates(CDC* pDC, CPoint &lTop, ChessColor currentPlayerColor);
    void DrawDesk(CDC* pDC, CPoint &lTop, ChessColor currentPlayerColor);
    void DrawChessmen(CDC* pDC, CPoint &lTop, ChessColor currentPlayerColor);

    HRESULT OnButtonOK(IHTMLElement *pElement);
    HRESULT OnButtonCancel(IHTMLElement *pElement);

protected:
    void DrawChessman(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect);
    int GetBmChessmanNumber(ChessmanValue value, ChessColor color);



    void DrawChessman__Resizeable(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect);
    int GetBmChessmanOffset(ChessmanValue value, ChessColor color);


    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

    DECLARE_MESSAGE_MAP()

    IDeckEngine *m_pDeckEngine;
    HICON m_hIcon;
    CBitmap m_bmChessmen;
    int m_nCellSize;
    int m_nCoordFieldWidth;

    CPoint m_StartPoint;

};
