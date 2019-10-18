#pragma once
#include "../Common/Includes.h"

//class Chessman;

class Player
{
public:
    Player();
    ~Player();

    int m_nPlayerColor;
    std::string m_sNickname;

    //std::vector<Chessman*> m_pRivalFigures;
};

