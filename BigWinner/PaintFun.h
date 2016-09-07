#pragma once

#include "windows.h"
#include <vector>
#include "BasicFun.h"

using namespace std;

#define BRUSH_PINK			RGB(247, 202, 201)
#define BRUSH_BLUE			RGB(146, 168, 209)
#define BRUSH_LIGHT_BLUE    RGB(153, 217, 234)
#define BRUSH_DEEP_BLUE		RGB(60, 160, 242)
#define BRUSH_RED           RGB(239, 99, 99)
#define BRUSH_WHITE			RGB(235, 235, 235)
#define BRUSH_GRAY			RGB(156, 156, 156)
#define BRUSH_BLACK			RGB(50, 50, 50)
#define BRUSH_LIGHT_GRAY	RGB(210, 210, 210)
#define BRUSH_DEEP_GRAY		RGB(173, 173, 173)

#define HORZION_COOR (0)
#define VERTICAL_COOR (1)

#define CHANGE_CNT 10	//window size change cnt

class PaintFun
{
public:
	PaintFun(void);
	~PaintFun(void);

	void DrawRect(HDC hdcBuffer, COLORREF color, RECT rect, COLORREF pen);
	void DrawFrame(HDC hdc, RECT rect);
	HFONT CreateMyFont(LPCTSTR face, int width, int height, int angle);
	HWND CreateButton(int x, int y, int width, int length, HWND parent_hwnd, HINSTANCE hInstance, int ID);
	void MoveButton(HWND hwnd, int x, int y, int width, int length, BOOL Repaint);
	void DrawButton(LPDRAWITEMSTRUCT pdis, TCHAR *text, int textnum);
};

