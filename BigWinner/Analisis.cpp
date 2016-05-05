#include "Analisis.h"

Analisis::Analisis(void)
{
	InterLen = 80;
	NewInterLen = 80;

	Pre_Mouse_Location.x = Pre_Mouse_Location.y = 0;
	Now_Mouse_Location.x = Now_Mouse_Location.y = 0;
	Mouse_xMove = 0;
	Point_Start_Location = 0;
}

Analisis::~Analisis(void)
{
}

void Analisis::GetFilePoint(OpFile *opfile)
{
	p_opfile = opfile;
}

void Analisis::ShowTable(HDC *hdc, OpFile *opfile)
{
	HDC *hdcBuffer = new HDC;
	HBITMAP *bitmapBuff = new HBITMAP;
	PaintFun paintfun;

	*bitmapBuff =  CreateCompatibleBitmap(*hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	*hdcBuffer = CreateCompatibleDC(*hdc);
	SelectObject(*hdcBuffer, *bitmapBuff);

	DrawBackGround(hdcBuffer);

	DrawCoordinate(hdcBuffer);

	BitBlt(*hdc, WorkRect.left, WorkRect.top, WorkRect.right, WorkRect.bottom, *hdcBuffer, WorkRect.left, WorkRect.top, SRCCOPY);

	DeleteObject(*bitmapBuff);
	DeleteDC(*hdcBuffer);
	delete(bitmapBuff);
	delete(hdcBuffer);
}

void Analisis::DrawCoordinate(HDC *hdcBuffer)
{
	PaintFun paintfun;
	int xCoor_Len = ShowRect.right - ShowRect.left - 50;
	int yCoor_Len = ShowRect.bottom - ShowRect.top - 20;
	POINT Start_Point = {ShowRect.left + 10, ShowRect.bottom - 20};
	static bool First_In = TRUE;
	int Total_Len = p_opfile->GetInfor_Capacity(DATA_NUM) * InterLen;
	int End_Point = - (Total_Len - xCoor_Len);

	if (First_In)
	{
		//start form the end point of the data
		Point_Start_Location = End_Point;
		First_In = FALSE;
	}

	if (Mouse_xMove != 0)
	{
		Point_Start_Location += Mouse_xMove;
		Mouse_xMove = 0;

		//restrict the Coordinate axes
		if (Point_Start_Location > 0)
		{
			Point_Start_Location = 0;
		}

		if (Point_Start_Location < End_Point)
		{
			Point_Start_Location = End_Point;
		}
	}

	vector<string>::iterator it_string = p_opfile->GetInfor_it(0, DATA_NUM);
	paintfun.DrawCoordinate(hdcBuffer, 
		Start_Point.x,
		Start_Point.y,
		Point_Start_Location, 
		xCoor_Len, 
		InterLen,
		p_opfile->GetInfor_Capacity(DATA_NUM),
		HORZION_COOR,
		it_string
		);

	paintfun.DrawCoordinate(hdcBuffer, 
		Start_Point.x,
		Start_Point.y,
		0, 
		ShowRect.bottom - ShowRect.top - 40, 
		(ShowRect.bottom - ShowRect.top - 40) / 33, 
		33,
		VERTICAL_COOR,
		it_string
		);
}

void Analisis::DrawBackGround(HDC *hdcBuffer)
{
	PaintFun paintfun;

	paintfun.DrawRect(hdcBuffer, BRUSH_BLUE, WorkRect, NULL_PEN);	//window background
	paintfun.DrawFrame(hdcBuffer, ShowRect);
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
	if (MouseWhell < 0)	//scroll down to zoom out
	{
		if (NewInterLen > 10)
		{
			NewInterLen -= 10;
		}
	}
	else if (MouseWhell > 0) //scroll up to zoom in
	{
		if (NewInterLen < 80)
		{
			NewInterLen += 10;
		}
	}
}

void Analisis::MouseAction(HWND hwnd, short x, short y, unsigned short act_type)
{
	switch(act_type)
	{
	case WM_LBUTTONDOWN:
		LButton_Down_Flag = TRUE;
		Slide_Flag = FALSE;
		SetCapture(hwnd);	//capture cursor outside window
		break;

	case WM_LBUTTONUP:
		LButton_Down_Flag = FALSE;
		Slide_xMove = (double)(Now_Mouse_Location.x - Pre_Mouse_Location.x);

		if (abs(Slide_xMove) >= 3)
		{
			Slide_Flag = TRUE;
		}
		ReleaseCapture();	//release cursor
		break;

	case WM_MOUSEMOVE:
		Pre_Mouse_Location = Now_Mouse_Location;
		Now_Mouse_Location.x = x;
		Now_Mouse_Location.y = y;
		//calculate moving pixel
		if (LButton_Down_Flag)
		{
			Mouse_xMove = Now_Mouse_Location.x - Pre_Mouse_Location.x;
			InvalidateRect(hwnd, &WorkRect, FALSE);
		}
		break;

	default:
		break;
	}
}

void Analisis::InvalidateArea(HWND hwnd)
{
	if (NewInterLen != InterLen)
	{
		if (NewInterLen > InterLen)
		{
			InterLen++;
		}
		else if (NewInterLen < InterLen)
		{
			InterLen--;
		}
		Zoom_Change = TRUE;
		InvalidateRect(hwnd, &WorkRect, FALSE);
	}
	else
	{
		Zoom_Change = FALSE;
	}

	if (Slide_Flag)
	{
		Mouse_xMove = (int)Slide_xMove;
		InvalidateRect(hwnd, &WorkRect, FALSE);
		Slide_xMove *= 0.9;
		if (abs(Slide_xMove) < 2)
		{
			Slide_Flag = FALSE;
			Slide_xMove = 0;
		}
	}
}

void Analisis::Exit()
{
}
