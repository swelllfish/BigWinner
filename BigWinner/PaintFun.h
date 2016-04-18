#pragma once

#include "windows.h"

#define BRUSH_PINK RGB(247, 202, 201)
#define BRUSH_BLUE RGB(146, 168, 209)
#define BRUSH_WHITE RGB(245, 245, 245)
#define BRUSH_GRAY RGB(156, 156, 156)
#define BRUSH_BLACK RGB(50, 50, 50)

#define HORZION_COOR (0)
#define VERTICAL_COOR (1)

#define CHANGE_CNT 10	//window size change cnt

class PaintFun
{
public:
	PaintFun(void);
	~PaintFun(void);

	void DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect, char pen);
	void DrawFrame(HDC *hdc, RECT rect);
	void DrawCoordinate(HDC *hdc, int xlocation, int ylocation, int len, int cnt, int start, unsigned char coortype);
};

