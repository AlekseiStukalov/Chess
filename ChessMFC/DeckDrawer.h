#pragma once
#include "../Common/Structures.h"


class DeckDrawer
{
public:
    DeckDrawer();
    ~DeckDrawer();
    void SetDeckParameters(int cellSize, int coordFieldWidth, int borderWidth);
    void SetCurrentPlayerColor(ChessColor color);
    CSize GetRequiredSize();

    void DrawBorder(CDC *pDC, CRect &clientRect);
    void DrawCoordinates(CDC* pDC);
    void DrawRawDesk(CDC* pDC);
protected:
    int m_nCellSize;
    int m_nCoordFieldWidth;
    int m_nBorderWidth;
    ChessColor m_CurrentPlayerColor;
};

