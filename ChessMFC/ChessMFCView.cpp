
// ChessMFCView.cpp : implementation of the CChessMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ChessMFC.h"
#endif

#include "ChessMFCDoc.h"
#include "ChessMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChessMFCView

IMPLEMENT_DYNCREATE(CChessMFCView, CView)

BEGIN_MESSAGE_MAP(CChessMFCView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CChessMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CChessMFCView construction/destruction

CChessMFCView::CChessMFCView()
{
	// TODO: add construction code here

}

CChessMFCView::~CChessMFCView()
{
}

BOOL CChessMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CChessMFCView drawing

void CChessMFCView::OnDraw(CDC* /*pDC*/)
{
	CChessMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CChessMFCView printing


void CChessMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CChessMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CChessMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CChessMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CChessMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CChessMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CChessMFCView diagnostics

#ifdef _DEBUG
void CChessMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CChessMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChessMFCDoc* CChessMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChessMFCDoc)));
	return (CChessMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CChessMFCView message handlers
