#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"

class IDeckEngine;

struct RedrawFlags
{
    void SetAll(bool bValue);

    bool bRedrawDesk;
    bool bRedrawMarks;
};

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

    void DrawCoordinates(CDC* pDC, ChessColor currentPlayerColor);
    void DrawRawDesk(CDC* pDC, ChessColor currentPlayerColor);
    void DrawDesk(CDC* pDC, ChessColor currentPlayerColor);
    void DrawChessmen(CDC* pDC, ChessColor currentPlayerColor);
    void DrawMarks(CDC* pDC, ChessColor currentPlayerColor);
    void DrawMark(CDC* pDC, ChessColor currentPlayerColor, std::vector<CellPos>& positions, COLORREF color);

protected:
    void DrawChessman(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect);
    int GetBmChessmanNumber(ChessmanValue value, ChessColor color);
    CRect GetRectByCellPos(CellPos &pos);

    void DrawChessman__Resizeable(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect);
    int GetBmChessmanOffset(ChessmanValue value, ChessColor color);

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    CBitmap m_bmChessmen;
    CImageList m_ilChessmen;
    RedrawFlags m_RedrawFlags;

    CPoint m_StartPoint;
    int m_nCellSize;
    int m_nCoordFieldWidth;
    CellPos m_SelectedCell;

    IDeckEngine *m_pDeckEngine;
    StepsPossibility m_StepsPossibility;
    ChessColor m_CurrentPlayerColor;
};
