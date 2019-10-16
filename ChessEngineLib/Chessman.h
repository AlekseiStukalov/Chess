#pragma once
#include "../Common/Structures.h"
#include "../Common/APIClasses.h"
#include "DeckCell.h"

class IDeckCell;
class DeckCell;

class Chessman : IChessman
{
public:
    Chessman();
    Chessman(ChessColor nChessmanColor, ChessmanValue nChessmanValue);
    ~Chessman();

	virtual IDeckCell* GetCurrentCell();
	virtual ChessColor GetChessmanColor();
	virtual ChessmanValue GetChessmanValue();
	virtual bool IsInitState();
	virtual bool IsKilled();
	virtual bool CanDoCastling();

	void SetCurrentCell(DeckCell *pCell);
	void SetChessmanColor(ChessColor nColor);
	void SetChessmanValue(ChessmanValue nValue);
	void SetCanDoCastling(bool bCastling);
	void SetKilled();
	void ResetInitState();

protected:

    DeckCell *m_pCurrentCell;

	ChessColor m_nChessmanColor;
    ChessmanValue m_nChessmanValue;

    bool m_bInitState;
    bool m_bCanDoCastling;  ///может сделать рокировку
    bool m_bKilled;
};