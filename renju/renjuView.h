
// renjuView.h : CrenjuView 类的接口
//

#pragma once
#include<utility>
#define analysed 15
#define NOCHESS 0
extern std::pair<int,int> ai(unsigned char a[][15],bool aifirst,int num);
class CrenjuView : public CView
{
protected: // 仅从序列化创建
	CrenjuView();
	DECLARE_DYNCREATE(CrenjuView)

// 特性
public:
	CrenjuDoc* GetDocument() const;
	//CImage img;
	CPen*ppen;
	CBrush*pbrush;
	CPoint origin;
	bool aifirst,start;
	int gridLength;
	int num;
	unsigned char matrix[15][15];
// 操作
public:
	bool forecast_a_line(unsigned char line[], int length, int pos)
	{
		int leftbound, rightbound = leftbound = pos;
		while (leftbound >= 1 && line[leftbound - 1] == line[pos])
		{
			leftbound--;
		}
		while (rightbound<length - 1 && line[rightbound + 1] == line[pos])
		{
			rightbound++;
		}
		int charlength = rightbound - leftbound + 1;
		if (charlength >= 5)
		{
			return true;
		}
		return false;
	}
	bool forecast_horizon(int row, int col)
	{
		return forecast_a_line(matrix[row], 15, col);
	}
	bool forecast_verti(int row, int col)
	{
		unsigned char column[15];
		for (int i = 0; i < 15; i++)
		{
			column[i] = matrix[i][col];
		}
		return forecast_a_line(column, 15, row);
	}
	bool forecast_downright(int row, int col)
	{
		unsigned char slash[15];
		int x, y;
		if (row<col)
		{
			x = 0;
			y = col - row;
		}
		else
		{
			y = 0;
			x = row - col;
		}
		int i;
		for (i = 0; x + i<15 && y + i<15; i++)
		{
			slash[i] = matrix[x + i][y + i];
		}
		return forecast_a_line(slash, i, col - y);
	}
	bool forecast_downleft(int row, int col)
	{
		unsigned char slash[15];
		int x, y;
		if (row + col < 15)
		{
			y = 0;
			x = row + col;
		}
		else
		{
			x = 14;
			y = row + col - 14;
		}
		int i;
		for (i = 0; x - i >= 0 && y + i<15; i++)
		{
			slash[i] = matrix[x - i][y + i];
		}
		return forecast_a_line(slash, i, col - y);
	}
	bool judge(int x, int y)
	{
		return (forecast_downleft(x, y) || forecast_downright(x, y) || forecast_horizon(x, y) || forecast_verti(x, y));
	}
	std::pair<int,int> aical(CDC* pdc)
	{
		std::pair<int, int> result = ai(matrix, aifirst, num);
		matrix[result.first][result.second] = aifirst ? 1 : 2;
		return result;
	}
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CrenjuView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void On32771();
	afx_msg void On32772();
};

#ifndef _DEBUG  // renjuView.cpp 中的调试版本
inline CrenjuDoc* CrenjuView::GetDocument() const
   { return reinterpret_cast<CrenjuDoc*>(m_pDocument); }
#endif

