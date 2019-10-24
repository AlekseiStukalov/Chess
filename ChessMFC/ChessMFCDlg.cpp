
// ChessMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChessMFC.h"
#include "ChessMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChessMFCDlg dialog

CChessMFCDlg::CChessMFCDlg(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_CHESSMFC_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChessMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

void CChessMFCDlg::DrawDesk()
{
    CImage pngImage;
    CBitmap pngBmp;
    CBitmap *pOldbmp;


    CDC bmDC;
    BITMAP  bi;
    UINT xPos = 384, yPos = 128;

    CClientDC dc(this);

    pngImage.LoadFromResource(AfxGetInstanceHandle(), IDB_CHESSMEN);

    IDB_CHESSMEN;

    CPaintDC dc(this);
    CPen *pen = new CPen(PS_SOLID, 3, RGB(0, 0, 0));
    dc.SelectObject(pen);
    dc.MoveTo(10, 10);
    dc.LineTo(10, 150);

    CRect rc(0, 0, 150, 150);
    CBrush br(RGB(0, 0, 0));

    FillRect(dc,rc, br);

    delete pen;
}

BEGIN_MESSAGE_MAP(CChessMFCDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CChessMFCDlg message handlers

BOOL CChessMFCDlg::OnInitDialog()
{
    CDialog::OnInitDialog();


    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChessMFCDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        DrawDesk();
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChessMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CChessMFCDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 600;
    lpMMI->ptMinTrackSize.y = 630;
}

HRESULT CChessMFCDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
    OnOK();
    return S_OK;
}

HRESULT CChessMFCDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
    OnCancel();
    return S_OK;
}
