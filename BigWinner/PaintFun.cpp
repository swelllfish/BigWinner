#include "PaintFun.h"


PaintFun::PaintFun(void)
{
}


PaintFun::~PaintFun(void)
{
}


void PaintFun::DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect)
{
	SelectObject(*hdcBuffer, CreateSolidBrush(color));
	Rectangle(*hdcBuffer, 
		rect.left, 
		rect.top, 
		rect.right, 
		rect.bottom
		);
}