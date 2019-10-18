// ChessEngineLib.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChessEngineLib.h"
#include "../Common/Defines.h"
#include "../Common/Includes.h"
#include "../Common/Structures.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChessEngineLibApp

BEGIN_MESSAGE_MAP(CChessEngineLibApp, CWinApp)
END_MESSAGE_MAP()


// CChessEngineLibApp construction

CChessEngineLibApp::CChessEngineLibApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


CChessEngineLibApp theApp;


BOOL CChessEngineLibApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}
