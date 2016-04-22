#include "Analisis.h"

Analisis::Analisis(void)
{
	PointCnt = 7;
	NewPointCnt = 7;

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

	if (Mouse_xMove)
	{
		int inter_len = (ShowRect.right - ShowRect.left - 20) / PointCnt;
		Point_Start_Location += Mouse_xMove;
		Mouse_xMove = 0;
		Point_Start_Location = Point_Start_Location > 0 ? Point_Start_Location % inter_len :Point_Start_Location % inter_len + inter_len;
	}

	vector<string>::iterator it_string = p_opfile->GetInfor_it(0, DATA_NUM);
	paintfun.DrawCoordinate(hdcBuffer, 
		ShowRect.left + 10,
		ShowRect.bottom - 10,
		ShowRect.right - ShowRect.left - 20, 
		PointCnt, 
		Point_Start_Location,
		HORZION_COOR
		);

	paintfun.DrawCoordinate(hdcBuffer, 
		ShowRect.left + 10,
		ShowRect.bottom - 10,
		ShowRect.bottom - ShowRect.top - 20, 
		33, 
		ShowRect.bottom - ShowRect.top - 20 / 33,
		VERTICAL_COOR
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
	if (MouseWhell < 0)	//scroll down
	{
		if (NewPointCnt < 170)
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

void Analisis::MouseAction(HWND hwnd, int x, int y, unsigned short act_type)
{
	switch(act_type)
	{
	case WM_LBUTTONDOWN:
		LButton_Down = TRUE;
		break;

	case WM_LBUTTONUP:
		if (Now_Mouse_Location.x - Pre_Mouse_Location.x == 0)
		{
			LButton_Down = FALSE;
		}
		break;

	case WM_MOUSEMOVE:
		Pre_Mouse_Location = Now_Mouse_Location;
		Now_Mouse_Location.x = x;
		Now_Mouse_Location.y = y;

		if (LButton_Down)
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