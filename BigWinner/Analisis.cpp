#include "Analisis.h"

Analisis::Analisis(void)
{
	nowInterLen = 80;
	newInterLen = 80;

	preMouseLocation.x = preMouseLocation.y = 0;
	nowMouseLocation.x = nowMouseLocation.y = 0;
	mouseMove_x = 0;
	tableStartPoint_x = 0;
}

Analisis::~Analisis(void)
{
}

void Analisis::CreateWindowButton(HWND hwnd, HINSTANCE hInstance)
{
	PaintFun paintfun;
	paintfun.CreateButton(tableAreaRect.right + 10, tableAreaRect.top, 80, 40, hwnd, hInstance, ID_BUTTON1);
}

void Analisis::DrawButton(LPDRAWITEMSTRUCT pdis)
{
	PaintFun paintfun;
	paintfun.DrawButton(pdis, TEXT("°´ÇòºÅÅÅÐò"), 5);
}

void Analisis::GetFilePoint(OpFile *opfile)
{
	p_opfile = opfile;
}

void Analisis::ShowTable(HDC hdc)
{
	HDC hdcBuffer;
	HBITMAP *bitmapBuff = new HBITMAP;
	HBITMAP PreBitmap;
	PaintFun paintfun;

	*bitmapBuff =  CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	hdcBuffer = CreateCompatibleDC(hdc);
	PreBitmap = (HBITMAP)SelectObject(hdcBuffer, *bitmapBuff);

	DrawBackGround(hdcBuffer);
	DrawCoordinate(hdcBuffer);

	BitBlt(hdc, windowAreaRect.left, windowAreaRect.top, windowAreaRect.right, windowAreaRect.bottom, hdcBuffer, windowAreaRect.left, windowAreaRect.top, SRCCOPY);

	SelectObject(hdcBuffer, PreBitmap);
	DeleteObject(*bitmapBuff);
	DeleteDC(hdcBuffer);
}

void Analisis::DrawCoordinate(HDC hdcBuffer)
{
	PaintFun paintfun;
	int xCoor_Len = tableAreaRect.right - tableAreaRect.left - 50;
	int yCoor_Len = tableAreaRect.bottom - tableAreaRect.top - 20;
	POINT Start_Point = {tableAreaRect.left + 10, tableAreaRect.bottom - 20};
	static bool First_In = TRUE;
	int Total_Len = p_opfile->GetInfor_Capacity(DATA_NUM) * nowInterLen;
	int End_Point = - (Total_Len - xCoor_Len);

	if (First_In)
	{
		//start form the end point of the data
		tableStartPoint_x = End_Point;
		First_In = FALSE;
	}

	if (mouseMove_x != 0)
	{
		tableStartPoint_x += mouseMove_x;
		mouseMove_x = 0;
	}

	if (zoomWhellChange != WHELL_ZOOM_NON)
	{
		int point_num = (whellMouseLocation.x - Start_Point.x - tableStartPoint_x) / preInterLen;

		if (zoomWhellChange == WHELL_ZOOM_IN)
		{
			tableStartPoint_x -= point_num * INTER_LEN_CHANGE;
		}
		else if (zoomWhellChange == WHELL_ZOOM_OUT)
		{
			tableStartPoint_x += point_num * INTER_LEN_CHANGE;
		}
	}

	//restrict the Coordinate axes
	if (tableStartPoint_x > 0)
	{
		tableStartPoint_x = 0;
	}

	if (tableStartPoint_x < End_Point)
	{
		tableStartPoint_x = End_Point;
	}

	vector<string>::iterator it_string = p_opfile->GetInfor_it(0, DATA_NUM);
	paintfun.DrawCoordinate(hdcBuffer, 
		Start_Point.x,
		Start_Point.y,
		tableStartPoint_x, 
		xCoor_Len, 
		nowInterLen,
		p_opfile->GetInfor_Capacity(DATA_NUM),
		HORZION_COOR,
		it_string
		);

	paintfun.DrawCoordinate(hdcBuffer, 
		Start_Point.x,
		Start_Point.y,
		0, 
		tableAreaRect.bottom - tableAreaRect.top - 40, 
		(tableAreaRect.bottom - tableAreaRect.top - 40) / 33, 
		33,
		VERTICAL_COOR,
		it_string
		);
}

void Analisis::DrawBackGround(HDC hdcBuffer)
{
	PaintFun paintfun;

	paintfun.DrawRect(hdcBuffer, BRUSH_BLUE, windowAreaRect, NULL_PEN);	//window background
	paintfun.DrawFrame(hdcBuffer, tableAreaRect);
}

void Analisis::SetWorkSpaceArea(int x, int y)
{
	windowAreaRect.top = 0;
	windowAreaRect.left = 0;
	windowAreaRect.right = x;
	windowAreaRect.bottom = y;

	tableAreaRect.top = windowAreaRect.top + 40;
	tableAreaRect.bottom = tableAreaRect.top + 49 * 17;  //every number has 20 pixel
	tableAreaRect.left = windowAreaRect.left + 50;
	tableAreaRect.right = windowAreaRect.right - 200;
}

void Analisis::ChangeShowArea(short MouseWhell)
{
	whellMouseLocation = nowMouseLocation;
	if (MouseWhell < 0)	//scroll down to zoom out
	{
		if (newInterLen > 10)
		{
			newInterLen -= 10;
		}
	}
	else if (MouseWhell > 0) //scroll up to zoom in
	{
		if (newInterLen < 80)
		{
			newInterLen += 10;
		}
	}
}

void Analisis::MouseAction(HWND hwnd, short x, short y, unsigned short act_type)
{
	switch(act_type)
	{
	case WM_LBUTTONDOWN:
		lButtonDownFlag = TRUE;
		tableSlideFlag = FALSE;
		SetCapture(hwnd);	//capture cursor outside window
		break;

	case WM_LBUTTONUP:
		lButtonDownFlag = FALSE;
		tableSlideMove_x = (double)(nowMouseLocation.x - preMouseLocation.x);

		if (abs(tableSlideMove_x) >= 3)
		{
			tableSlideFlag = TRUE;
		}
		ReleaseCapture();	//release cursor
		break;

	case WM_MOUSEMOVE:
		preMouseLocation = nowMouseLocation;
		nowMouseLocation.x = x;
		nowMouseLocation.y = y;
		//calculate moving pixel
		if (lButtonDownFlag)
		{
			mouseMove_x = nowMouseLocation.x - preMouseLocation.x;
			InvalidateRect(hwnd, &tableAreaRect, FALSE);
		}
		break;

	default:
		break;
	}
}

void Analisis::InvalidateArea(HWND hwnd)
{
	if (newInterLen != nowInterLen)
	{
		preInterLen = nowInterLen;

		if (newInterLen > nowInterLen)
		{
			nowInterLen += INTER_LEN_CHANGE;
			zoomWhellChange = WHELL_ZOOM_IN;
		}
		else if (newInterLen < nowInterLen)
		{
			nowInterLen -= INTER_LEN_CHANGE;
			zoomWhellChange = WHELL_ZOOM_OUT;
		}
		InvalidateRect(hwnd, &tableAreaRect, FALSE);
	}
	else
	{
		zoomWhellChange = WHELL_ZOOM_NON;
	}

	if (tableSlideFlag)
	{
		mouseMove_x = (int)tableSlideMove_x;
		InvalidateRect(hwnd, &tableAreaRect, FALSE);
		tableSlideMove_x *= 0.9;
		if (abs(tableSlideMove_x) < 2)
		{
			tableSlideFlag = FALSE;
			tableSlideMove_x = 0;
		}
	}
}

void Analisis::Exit()
{
}
