#include "Analisis.h"

Analisis::Analisis(void)
{
}

Analisis::~Analisis(void)
{
}

void Analisis::ShowTable(HDC hdc, OpFile *opfile)
{
	DrawBackGround(hdc);
}

void Analisis::SetBackGroundBM(HDC hdc)
{
	BMBackGround =  CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

void Analisis::DrawBackGround(HDC hdc)
{
	HDC *hdcBuffer = new HDC;

	*hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(*hdcBuffer, BMBackGround);
	DrawRect(hdcBuffer, BRUSH_BLUE, WorkRect, NULL_PEN);	//window background
	DrawFrame(hdcBuffer, ShowRect);
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
