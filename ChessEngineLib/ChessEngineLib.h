#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "Deck.h"

class CChessEngineLibApp : public CWinApp
{
public:
	CChessEngineLibApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
