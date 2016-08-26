#include "PaintFun.h"


PaintFun::PaintFun(void)
{
}


PaintFun::~PaintFun(void)
{
}


void PaintFun::DrawRect(HDC hdcBuffer, COLORREF color, RECT rect, COLORREF pen)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_SOLID, 1, pen);
	HPEN hPrePen = (HPEN)SelectObject(hdcBuffer, hPen);
	HBRUSH hPreBrush = (HBRUSH)SelectObject(hdcBuffer, hBrush);
	Rectangle(hdcBuffer, 
		rect.left, 
		rect.top, 
		rect.right, 
		rect.bottom
		);
	SelectObject(hdcBuffer, hPrePen);
	DeleteObject(hPen);
	SelectObject(hdcBuffer, hPreBrush);
	DeleteObject(hBrush);
}

void PaintFun::DrawFrame(HDC hdcBuffer, RECT rect)
{
	rect.left += 3;
	rect.top += 3;
	rect.right += 3; 
	rect.bottom += 3;
	DrawRect(hdcBuffer, BRUSH_BLACK, rect, BRUSH_BLACK);
	rect.left -= 4;
	rect.top -= 4;
	rect.right -= 2; 
	rect.bottom -= 2;
	DrawRect(hdcBuffer, BRUSH_GRAY, rect, BRUSH_GRAY);
	rect.left += 1;
	rect.top += 1;
	rect.right -= 1; 
	rect.bottom -= 1;
	DrawRect(hdcBuffer, BRUSH_WHITE, rect, BRUSH_WHITE);
}


HFONT PaintFun::CreateMyFont(LPCTSTR face, int width, int height, int angle)
{
	HFONT hFont;
	hFont = CreateFont(
		height,      //字体的逻辑高度
		width,       //逻辑平均字符宽度
		angle,       //与水平线的角度
		0,           //基线方位角度
		FW_REGULAR,  //字形：常规
		FALSE,       //字形：斜体
		FALSE,       //字形：下划线
		FALSE,       //字形：粗体
		GB2312_CHARSET,          //字符集
		OUT_DEFAULT_PRECIS,      //输出精度
		CLIP_DEFAULT_PRECIS,     //剪截精度
		PROOF_QUALITY,           //输出品质
		FIXED_PITCH | FF_MODERN, //倾斜度
		face                     //字体
		); 
	return hFont;
}

HWND PaintFun::CreateButton(int x, int y, int width, int length, HWND parent_hwnd, HINSTANCE hInstance, int ID)
{
	return CreateWindow(TEXT("button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, length, parent_hwnd, (HMENU)ID, hInstance, NULL);
}

void PaintFun::MoveButton(HWND hwnd, int x, int y, int width, int length, BOOL Repaint)
{
	MoveWindow(hwnd, x, y, width, length, Repaint);
}

void PaintFun::DrawButton(LPDRAWITEMSTRUCT pdis, TCHAR *text, int textnum)
{
	TEXTMETRIC tm;
	GetTextMetrics(pdis->hDC, &tm);
	int xText = ((pdis->rcItem.right - pdis->rcItem.left) / 2) - (13 * textnum) / 2;
	int yText = (pdis->rcItem.bottom - pdis->rcItem.top) / 2 - 13 / 2;
	DrawRect(pdis->hDC, BRUSH_LIGHT_GRAY, pdis->rcItem, BRUSH_DEEP_GRAY);
	SetBkColor(pdis->hDC, BRUSH_LIGHT_GRAY);
	
	HFONT hFont, hPreFont;
	hFont = CreateMyFont((LPCTSTR)("微软雅黑"), 7, 13, 0);
	hPreFont = (HFONT)SelectObject(pdis->hDC, hFont);
	TextOut(pdis->hDC, xText, yText, text, textnum);

	if (pdis->itemState & ODS_SELECTED)
	{
		DrawRect(pdis->hDC, BRUSH_DEEP_GRAY, pdis->rcItem, BRUSH_DEEP_GRAY);
		SetBkColor(pdis->hDC, BRUSH_DEEP_GRAY);
		TextOut(pdis->hDC, xText, yText, text, textnum);
	}
	SelectObject(pdis->hDC, hPreFont);
	DeleteObject(hFont);
}
