
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

void CChessMFCDlg::DrawDesk(CPoint &lTop, CPoint &rBottom)
{
    CPaintDC dc(this);
    CImageList imageList;
    imageList.Create(IDB_CHESSMEN, 64, 0, RGB(0, 0, 0));

    //draw line
    //CPen *pen = new CPen(PS_SOLID, 3, RGB(0, 0, 0));
    //dc.SelectObject(pen);
    //dc.MoveTo(10, 10);
    //dc.LineTo(10, 150);
    //delete pen;

    CRect rc(0, 0, 150, 150);
    FillRect(dc, rc, CBrush(RGB(150, 60, 0)));

    //imageList.Draw(&dc, 0, CPoint(0, 0), ILD_TRANSPARENT);



    IMAGEINFO iinfo;
    imageList.GetImageInfo(1, &iinfo);

    CBitmap *pBitmap = CBitmap::FromHandle(iinfo.hbmImage);


    CDC MemDC;
    MemDC.CreateCompatibleDC(&dc);
    CBitmap* pOldBitmap = dc.SelectObject(pBitmap);

    int bmw, bmh;
    BITMAP bmap;
    pBitmap->GetBitmap(&bmap);
    bmw = bmap.bmWidth;
    bmh = bmap.bmHeight;
    int xo = 0, yo = 0;

    dc.StretchBlt(xo, yo, 32,
        32, &MemDC,
        0, 0, bmw, bmh, SRCCOPY);

    MemDC.SelectObject(pOldBitmap);



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
        DrawDesk(CPoint(0,0), CPoint(500, 500));
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
