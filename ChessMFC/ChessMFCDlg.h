#pragma once


class CChessMFCDlg : public CDialog
{
public:
    CChessMFCDlg(CWnd* pParent = NULL);

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHESSMFC_DIALOG, IDH = IDR_HTML_CHESSMFC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    void DrawDesk();

    HRESULT OnButtonOK(IHTMLElement *pElement);
    HRESULT OnButtonCancel(IHTMLElement *pElement);

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

    DECLARE_MESSAGE_MAP()
};
