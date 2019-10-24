#pragma once
#include "../Common/Structures.h"
#include "Chessman.h"

class Chessman;

//sizeof = 72(48)
class DeckCell : public IDeckCell
{
public:
    DeckCell();
    ~DeckCell();

    virtual ChessColor GetCellColor() const;
    virtual IChessman* GetChessman() const;
    virtual std::string GetCellName() const;
    virtual CellPos GetCellPos() const;

    void SetCellColor(ChessColor color);
    void SetChessman(Chessman *pChessman);
    void SetCellInfo(std::string &cellName);

protected:
    ChessColor m_nCellColor;
    std::string m_sCellName;
    CellPos m_CellPos;
    Chessman* m_pChessman;
};

