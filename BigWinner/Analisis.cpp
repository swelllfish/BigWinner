#include "Analisis.h"

#define BRUSH_PINK RGB(247, 202, 201)
#define BRUSH_BLUE RGB(146, 168, 209)
#define BRUSH_WHITE RGB(245, 245, 245)
#define BRUSH_GRAY RGB(156, 156, 156)
#define BRUSH_BLACK RGB(50, 50, 50)

#define CHANGE_CNT 10	//window size change cnt

Analisis::Analisis(void)
{
}


Analisis::~Analisis(void)
{
}

void Analisis::SetBackGroundBM(HDC hdc)
{
	BMBackGround =  CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void Analisis::DrawBackGround(HDC hdc)
{
	static short cnt = 0;
	HDC *hdcBuffer = new HDC;

	*hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(*hdcBuffer, BMBackGround);
	SelectObject(*hdcBuffer, GetStockObject(NULL_PEN));

	DrawRect(hdcBuffer, BRUSH_BLUE, WorkRect);	//window background

	ShowRect.left += 3;
	ShowRect.top += 3;
	ShowRect.right += 3; 
	ShowRect.bottom += 3;
	DrawRect(hdcBuffer, BRUSH_BLACK, ShowRect);
	ShowRect.left -= 3;
	ShowRect.top -= 3;
	ShowRect.right -= 3; 
	ShowRect.bottom -= 3;

	ShowRect.left -= 1;
	ShowRect.top -= 1;
	ShowRect.right += 1; 
	ShowRect.bottom += 1;
	DrawRect(hdcBuffer, BRUSH_GRAY, ShowRect);

	ShowRect.left += 1;
	ShowRect.top += 1;
	ShowRect.right -= 1; 
	ShowRect.bottom -= 1;

	DrawRect(hdcBuffer, BRUSH_WHITE, ShowRect);

	BitBlt(hdc, WorkRect.left, WorkRect.top, WorkRect.right, WorkRect.bottom, *hdcBuffer, WorkRect.left, WorkRect.top, SRCCOPY);

	free(hdcBuffer);
}

void Analisis::SetWorkSpaceArea(int x, int y, HDC hdc)
{
	WorkRect.top = 0;
	WorkRect.left = 0;
	WorkRect.right = x;
	WorkRect.bottom = y;

	ShowRect.top = WorkRect.bottom / 5;
	ShowRect.bottom = WorkRect.top + 33 * 20;  //every number has 20 pixel
	ShowRect.left = WorkRect.left + 50;
	ShowRect.right = WorkRect.right - 50;

	MouseWhellFlag = FALSE;

	SizeRate = 10;
}

void Analisis::ChangeShowArea(short MouseWhell)
{
	if (MouseWhell < 0)	//scroll down
	{
	}
	else if (MouseWhell > 0) //scroll up
	{
	}
}

void Analisis::InvalidateArea(HWND hwnd)
{
	InvalidateRect(hwnd, &WorkRect, FALSE);
}

void Analisis::Exit()
{
	DeleteObject(BMBackGround);
}
