#include "Analisis.h"

#define PINK RGB(247, 202, 201)
#define BLUE RGB(146, 168, 209)
#define WHITE RGB(245, 245, 245)
#define GRAY RGB(106, 104, 94)

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

	if (MouseWhellFlag)
	{
		DrawRect(hdcBuffer, BLUE, WorkRect);

		ShowRect.left += ((NewRect.left - ShowRect.left) / CHANGE_CNT);
		ShowRect.right += ((NewRect.right - ShowRect.right)  / CHANGE_CNT);

		ShowRect.left -= 2;
		ShowRect.top -= 2;
		ShowRect.right += 2; 
		ShowRect.bottom += 2;
		DrawRect(hdcBuffer, GRAY, ShowRect);

		ShowRect.left += 2;
		ShowRect.top += 2;
		ShowRect.right -= 2; 
		ShowRect.bottom -= 2;

		DrawRect(hdcBuffer, WHITE, ShowRect);

	}
	else
	{
		DrawRect(hdcBuffer, BLUE, WorkRect);	//window background

		ShowRect.left -= 2;
		ShowRect.top -= 2;
		ShowRect.right += 2; 
		ShowRect.bottom += 2;
		DrawRect(hdcBuffer, GRAY, ShowRect);

		ShowRect.left += 2;
		ShowRect.top += 2;
		ShowRect.right -= 2; 
		ShowRect.bottom -= 2;

		DrawRect(hdcBuffer, WHITE, ShowRect);
	}

	BitBlt(hdc, WorkRect.left, WorkRect.top, WorkRect.right, WorkRect.bottom, *hdcBuffer, WorkRect.left, WorkRect.top, SRCCOPY);

	DeleteDC(*hdcBuffer);
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
	NewRect = ShowRect;

	MouseWhellFlag = FALSE;

	SizeRate = 10;
}

void Analisis::ChangeShowArea(short MouseWhell)
{
	if (MouseWhell < 0)	//scroll down
	{
		if (SizeRate > 1)
		{	
			--SizeRate;

			NewRect.left = WorkRect.right / 2 - ((WorkRect.right - 100) * SizeRate / 10) / 2;
			NewRect.right = WorkRect.right / 2 + ((WorkRect.right - 100) * SizeRate / 10) / 2;	
		}
		else
		{
			return ;
		}
	}
	else if (MouseWhell > 0) //scroll up
	{
		if (SizeRate < 10)
		{	
			++SizeRate;

			NewRect.left = WorkRect.right / 2 - ((WorkRect.right - 100) * SizeRate / 10) / 2;
			NewRect.right = WorkRect.right / 2 + ((WorkRect.right - 100) * SizeRate / 10) / 2;			
		}
		else
		{
			return ;
		}
	}
}

void Analisis::InvalidateArea(HWND hwnd)
{
	if (((NewRect.right - ShowRect.right)  / CHANGE_CNT) != 0)
	{
		MouseWhellFlag = TRUE;
		InvalidateRect(hwnd, &WorkRect, FALSE);
	}
}

void Analisis::Exit()
{
	DeleteObject(BMBackGround);
}
