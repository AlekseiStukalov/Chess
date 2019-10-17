#pragma once
#include "../Common/Structures.h"
#include "Chessman.h"

class Chessman;

class DeckCell : public IDeckCell
{
public:
    DeckCell();
    ~DeckCell();

    virtual ChessColor GetCellColor();
    virtual IChessman* GetChessman();
    virtual std::string GetCellName();
    virtual CellPos GetCellPos();

    void SetCellColor(ChessColor color);
    void SetChessman(Chessman *pChessman);
    void SetCellName(std::string cellName);

protected:
    ChessColor m_nCellColor;
    std::string m_sCellName;
    Chessman* m_pChessman;
};

