
// renjuView.cpp : CrenjuView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "renju.h"
#endif

#include "renjuDoc.h"
#include "renjuView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrenjuView

IMPLEMENT_DYNCREATE(CrenjuView, CView)

BEGIN_MESSAGE_MAP(CrenjuView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_32771, &CrenjuView::On32771)
	ON_COMMAND(ID_32772, &CrenjuView::On32772)
END_MESSAGE_MAP()

// CrenjuView ����/����

CrenjuView::CrenjuView()
{
	// TODO: �ڴ˴���ӹ������
}

CrenjuView::~CrenjuView()
{
}

BOOL CrenjuView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CrenjuView ����

void CrenjuView::OnDraw(CDC* pDC)
{
	CrenjuDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CRect rect;
	GetClientRect(&rect);
	CImage img;
	img.Load(L"res\\background.jpg");
	img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height());
	ppen = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen*pOldPen = pDC->SelectObject(ppen);
	for (int i = origin.x; i <= 16*gridLength; i+=gridLength)
	{
		pDC->MoveTo(i, origin.y);
		pDC->LineTo(i, 15*gridLength+origin.y);
	}
	for (int i = origin.y; i <= 16*gridLength; i += gridLength)
	{
		pDC->MoveTo(origin.x, i);
		pDC->LineTo(15*gridLength+origin.x,i);
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (matrix[i][j] != 0)
			{
				if (matrix[i][j] == 1)
				{
					pbrush = new CBrush(RGB(0, 0, 0));
				}
				else
				{
					pbrush = new CBrush(RGB(255, 255, 255));
				}
				CBrush*pOld = pDC->SelectObject(pbrush);
				pDC->Ellipse(40 * i + 10, 40 * j + 10, 40 * i + 50, 40 * j + 50);
				pDC->SelectObject(pOld);
				pbrush->DeleteObject();
			}
		}
	}
	pDC->SelectObject(pOldPen);
	ppen->DeleteObject();
}


// CrenjuView ��ӡ

BOOL CrenjuView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CrenjuView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CrenjuView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CrenjuView ���

#ifdef _DEBUG
void CrenjuView::AssertValid() const
{
	CView::AssertValid();
}

void CrenjuView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrenjuDoc* CrenjuView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrenjuDoc)));
	return (CrenjuDoc*)m_pDocument;
}
#endif //_DEBUG


// CrenjuView ��Ϣ�������


int CrenjuView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	memset(matrix, 0, sizeof(matrix));
	ppen = nullptr;
	pbrush = nullptr;
	gridLength = 40;
	origin = CPoint(10, 10);
	start = false;
	num = 0;
	return 0;
}


void CrenjuView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(start)
	if (point.x > 10 && point.x < 610 && point.y>10 && point.y < 610)
	{
		int x = (point.x - 10) / 40;
		int y = (point.y - 10) / 40;
		//pdc->MoveTo(40 * x + 10, 40 * y + 10);
		if (matrix[x][y] == 0)
		{
			num++;
			CDC*pdc = GetDC();
			if (aifirst)
			{
				matrix[x][y] = 2;
				pbrush = new CBrush(RGB(255, 255, 255));
			}
			else
			{
				matrix[x][y] = 1;
				pbrush = new CBrush(RGB(0, 0, 0));
			}
			CBrush*pOld = pdc->SelectObject(pbrush);
			pdc->Ellipse(40 * x + 10, 40 * y + 10, 40 * x + 50, 40 * y + 50);
			pdc->SelectObject(pOld);
			pbrush->DeleteObject();
			if (judge(x, y))
			{
				start = false;
				MessageBox(L"��Ӯ�ˣ�����", L"���");
			}
			else
			{
				std::pair<int, int> temp = aical(pdc);
				//Sleep(3000);
				x = temp.first; y = temp.second;
				if (aifirst)
				{
					pbrush = new CBrush(RGB(0, 0, 0));
				}
				else
				{
					pbrush = new CBrush(RGB(255, 255, 255));
				}
				pOld = pdc->SelectObject(pbrush);
				pdc->Ellipse(40 * x + 10, 40 * y + 10, 40 * x + 50, 40 * y + 50);
				pdc->SelectObject(pOld);
				pbrush->DeleteObject();
				if (judge(x, y))
				{
					start = false;
					MessageBox(L"������~~~", L"���");
				}
			}
			ReleaseDC(pdc);
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CrenjuView::On32771()
{
	// TODO: �ڴ���������������
	start = true;
	aifirst = false;
	num = 0;
	memset(matrix, 0, sizeof(matrix));
	RedrawWindow();
}


void CrenjuView::On32772()
{
	// TODO: �ڴ���������������
	start = true;
	aifirst = true;
	num = 0;
	memset(matrix, 0, sizeof(matrix));
	RedrawWindow();
	CDC*pdc = GetDC();
	std::pair<int, int> temp = aical(pdc);
	int x = temp.first, y = temp.second;
	pbrush = new CBrush(RGB(0, 0, 0));
	CBrush* pOld = pdc->SelectObject(pbrush);
	pdc->Ellipse(40 * x + 10, 40 * y + 10, 40 * x + 50, 40 * y + 50);
	pdc->SelectObject(pOld);
	pbrush->DeleteObject();
	ReleaseDC(pdc);
	num++;
}
