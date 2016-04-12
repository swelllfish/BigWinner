#pragma once

#include "windows.h"

class PaintFun
{
public:
	PaintFun(void);
	~PaintFun(void);

	void DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect);
};

