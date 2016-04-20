#include "Analisis.h"

Analisis::Analisis(void)
{
	PointCnt = 7;
	NewPointCnt = 7;
}

Analisis::~Analisis(void)
{
}

void Analisis::ShowTable(HDC *hdc, OpFile *opfile)
{
	HDC *hdcBuffer = new HDC;
	HBITMAP *bitmapBuff = new HBITMAP;

	*bitmapBuff =  CreateCompatibleBitmap(*hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	*hdcBuffer = CreateCompatibleDC(*hdc);
	SelectObject(*hdcBuffer, *bitmapBuff);

	DrawBackGround(hdcBuffer);

	DrawCoordinate(hdcBuffer, 
		ShowRect.left + 10,
		ShowRect.bottom - 10,
		ShowRect.right - ShowRect.left - 20, 
		PointCnt, 
		50,
		HORZION_COOR
		);

	DrawCoordinate(hdcBuffer, 
		ShowRect.left + 10,
		ShowRect.bottom - 10,
		ShowRect.bottom - ShowRect.top - 20, 
		33, 
		100,
		VERTICAL_COOR
		);

	BitBlt(*hdc, WorkRect.left, WorkRect.top, WorkRect.right, WorkRect.bottom, *hdcBuffer, WorkRect.left, WorkRect.top, SRCCOPY);

	DeleteObject(*bitmapBuff);
	DeleteDC(*hdcBuffer);
	delete(bitmapBuff);
	delete(hdcBuffer);
}

void Analisis::DrawBackGround(HDC *hdcBuffer)
{
	DrawRect(hdcBuffer, BRUSH_BLUE, WorkRect, NULL_PEN);	//window background
	DrawFrame(hdcBuffer, ShowRect);
}

void Analisis::SetWorkSpaceArea(int x, int y)
{
	WorkRect.top = 0;
	WorkRect.left = 0;
	WorkRect.right = x;
	WorkRect.bottom = y;

	ShowRect.top = WorkRect.bottom / 5;
	ShowRect.bottom = ShowRect.top + 33 * 20;  //every number has 20 pixel
	ShowRect.left = WorkRect.left + 50;
	ShowRect.right = WorkRect.right - 50;
}

void Analisis::ChangeShowArea(short MouseWhell)
{
	if (MouseWhell < 0)	//scroll down
	{
		if (NewPointCnt < 490)
		{
			NewPointCnt += 7;
		}
	}
	else if (MouseWhell > 0) //scroll up
	{
		if (NewPointCnt > 7)
		{
			NewPointCnt -= 7;
		}
	}
}

void Analisis::InvalidateArea(HWND hwnd)
{
	if (NewPointCnt != PointCnt)
	{
		if (NewPointCnt > PointCnt)
		{
			PointCnt++;
		}
		else if (NewPointCnt < PointCnt)
		{
			PointCnt--;
		}
		InvalidateRect(hwnd, &WorkRect, FALSE);
	}
}

void Analisis::Exit()
{
}
