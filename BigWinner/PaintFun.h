#pragma once

#include "windows.h"
#include <vector>
#include "BasicFun.h"

using namespace std;

#define BRUSH_PINK			RGB(247, 202, 201)
#define BRUSH_BLUE			RGB(146, 168, 209)
#define BRUSH_WHITE			RGB(245, 245, 245)
#define BRUSH_GRAY			RGB(156, 156, 156)
#define BRUSH_BLACK			RGB(50, 50, 50)
#define BRUSH_LIGHT_GRAY	RGB(240, 240, 240)
#define BRUSH_DEEP_GRAY		RGB(173, 173, 173)

#define HORZION_COOR (0)
#define VERTICAL_COOR (1)

#define CHANGE_CNT 10	//window size change cnt

class PaintFun
{
public:
	PaintFun(void);
	~PaintFun(void);

	void DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect, COLORREF pen);
	void DrawFrame(HDC *hdc, RECT rect);
	void DrawCoordinate(HDC *hdcBuffer, 
	int xlocation, 
	int ylocation, 
	int start, 
	int len,
	int inter_len,
	int total_cnt, 
	unsigned char coortype,
	vector<string>::iterator TextString);
	HFONT CreateMyFont(HDC hdc, LPCTSTR face, int width, int height, int angle);
	HWND CreateButton(int x, int y, int width, int length, HWND parent_hwnd, HINSTANCE hInstance, int ID);
	void DrawButton(LPDRAWITEMSTRUCT pdis, TCHAR *text, int textnum);
};

