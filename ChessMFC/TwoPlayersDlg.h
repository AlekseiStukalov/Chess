#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "DeckDrawer.h"

#define DEFAULT_DECK_SIZE 572;

struct RedrawFlags
{
    void SetAll(bool bValue);

    bool bRedrawDesk;
    bool bRedrawMarks;
};

class CTwoPlayersDlg : public CDialog
{
	DECLARE_DYNAMIC(CTwoPlayersDlg)

public:
	CTwoPlayersDlg(CWnd* pParent = NULL);
    virtual ~CTwoPlayersDlg();
    virtual BOOL OnInitDialog();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    void SetEngine(IDeckEngine *pDeckEngine);
    void SetCurrentPlayerColor(ChessColor currentPlayerColor);
    CSize GetRequiredSize();

    void DrawDesk(CDC* pDC);
    void DrawChessmen(CDC* pDC);
    void DrawChessman__Resizeable(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect);
    void DrawChessman(CDC* pDC, ChessmanValue value, ChessColor color, CRect rect);
    void DrawMarks(CDC* pDC);
    void DrawMark(CDC* pDC, std::vector<CellPos>& positions, COLORREF color);

    int GetBmChessmanOffset(ChessmanValue value, ChessColor color);
    int GetBmChessmanNumber(ChessmanValue value, ChessColor color);
    CRect GetRectByCellPos(CellPos &pos);

    HBITMAP CreateBitmapMask(HBITMAP hbmColour);
protected:
    RedrawFlags m_RedrawFlags;
    CBitmap m_bmChessmen;
    CImageList m_ilChessmen;
    int m_nCellSize;
    int m_nCoordFieldWidth;
    int m_nBorderWidth;

    DeckDrawer m_DeckDrawer;
    ChessColor m_CurrentPlayerColor;
    CellPos m_SelectedCell;
    StepsPossibility m_StepsPossibility;
    IDeckEngine *m_pEngine;

    DECLARE_MESSAGE_MAP()
};


