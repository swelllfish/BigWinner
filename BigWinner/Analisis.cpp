#include "Analisis.h"

Analisis::Analisis(void)
{
}


Analisis::~Analisis(void)
{
}


void Analisis::DrawBackGround(HDC hdc)
{
	static short cnt = 0;

	SelectObject(hdc, CreateSolidBrush(RGB(247, 202, 201)));
	SelectObject(hdc, GetStockObject(NULL_PEN));

	if (MouseWhellFlag)
	{
		if (cnt < 10)
		{
			ShowRect.left += ((NewRect.left - ShowRect.left) / 10);
			ShowRect.right += ((NewRect.right - ShowRect.right)  / 10);


			Rectangle(hdc, 
				ShowRect.left,
				ShowRect.top, 
				ShowRect.right, 
				ShowRect.bottom
				);

			++cnt;
			if (cnt == 10)
			{
				MouseWhellFlag = FALSE;
				cnt = 0;
			}
		}
	}
	else
	{
		Rectangle(hdc, ShowRect.left,ShowRect.top, ShowRect.right, ShowRect.bottom);
	}
}

void Analisis::SetWorkSpaceArea(int x, int y, HDC hdc)
{
	WorkRect.top = 0;
	WorkRect.left = 0;
	WorkRect.right = x;
	WorkRect.bottom = y;

	ShowRect.top = WorkRect.bottom / 5;
	ShowRect.bottom = WorkRect.top + 33 * 20;  //every number has 5 pixel
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
	if (ShowRect.left != NewRect.left && ShowRect.right != NewRect.right)
	{
		MouseWhellFlag = TRUE;
		InvalidateRect(hwnd, &WorkRect, TRUE);
	}
}