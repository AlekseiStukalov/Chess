#pragma once


class CChessMenu : public CDialog
{
    DECLARE_DYNAMIC(CChessMenu)
    DECLARE_MESSAGE_MAP()

public:
    CChessMenu(CWnd* pParent = NULL);   // standard constructor
    virtual ~CChessMenu();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnStart2Players();
    afx_msg void OnStartProblemSolving();
};
