#include "Analisis.h"

Analisis::Analisis(void)
{
	nowInterLen = 80;
	newInterLen = 80;

	preMouseLocation.x = preMouseLocation.y = 0;
	nowMouseLocation.x = nowMouseLocation.y = 0;
	mouseMove_x = 0;
	tableStartPoint_y = 0;
}

Analisis::~Analisis(void)
{
}

void Analisis::CreateWindowButton(HWND hwnd, HINSTANCE hInstance)
{
	PaintFun paintfun;

	Analysis_Button.Button1 = paintfun.CreateButton(0, 0, 80, 40, hwnd, hInstance, ID_BUTTON1);
}

void Analisis::MoveWindowButton()
{
	PaintFun paintfun;

	paintfun.MoveButton(Analysis_Button.Button1, tableAreaRect.right + 10, tableAreaRect.top, 80, 40, TRUE);
}

void Analisis::DrawButton(LPDRAWITEMSTRUCT pdis)
{
	PaintFun paintfun;
	paintfun.DrawButton(pdis, TEXT("按球号排序"), 5);
}

void Analisis::GetFilePoint(OpFile *opfile)
{
	p_opfile = opfile;
}

void Analisis::ShowTable(HDC hdc)
{
	HDC hdcBuffer;
	HBITMAP bitmapBuff;
	HBITMAP PreBitmap;
	PaintFun paintfun;

// 	LARGE_INTEGER begin_time, end_time, freq_;
// 	QueryPerformanceFrequency(&freq_);
// 	QueryPerformanceCounter(&begin_time);

	bitmapBuff =  CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	hdcBuffer = CreateCompatibleDC(hdc);
	PreBitmap = (HBITMAP)SelectObject(hdcBuffer, bitmapBuff);

	DrawBackGround(hdcBuffer);
	DrawCoordinate(hdcBuffer);
	BitBlt(hdc, windowAreaRect.left, windowAreaRect.top, windowAreaRect.right, windowAreaRect.bottom, hdcBuffer, windowAreaRect.left, windowAreaRect.top, SRCCOPY);

	SelectObject(hdcBuffer, PreBitmap);
	DeleteObject(bitmapBuff);
	DeleteDC(hdcBuffer);

// 	QueryPerformanceCounter(&end_time);
// 	double ns_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
// 	char buffer[20];
// 	sprintf_s(buffer, "%f", ns_time);
// 	string time = buffer;
// 	wchar_t *lpcText;
// 	lpcText = (wchar_t *) malloc(sizeof(wchar_t) *time.length());
// 	StringToLPCWSTR(time, lpcText);
// 	TextOut(hdc, tableAreaRect.left, tableAreaRect.top, lpcText, 12);
}

void Analisis::DrawCoordinate(HDC hdcBuffer)
{
	PaintFun paintfun;
	int xCoor_Len = 1000/*tableAreaRect.right - tableAreaRect.left - 120*/;
	int yCoor_Len = tableAreaRect.bottom - tableAreaRect.top - 40;
	POINT Start_Point = {tableAreaRect.left + 80, tableAreaRect.bottom - 20};
	static bool First_In = TRUE;
	int Total_Len = p_opfile->GetInfor_Capacity(DATA_NUM) * nowInterLen;
	int End_Point = - (Total_Len - yCoor_Len);

	if (First_In)
	{
		//start form the end point of the data
		tableStartPoint_y = End_Point;
		First_In = FALSE;
	}

	if (mouseMove_y != 0)
	{
		tableStartPoint_y -= mouseMove_y;
		mouseMove_y = 0;
	}

	if (zoomWhellChange != WHELL_ZOOM_NON)
	{
		int point_num = (whellMouseLocation.y - (Start_Point.y - yCoor_Len + tableStartPoint_y)) / preInterLen;

		if (zoomWhellChange == WHELL_ZOOM_IN)
		{
			tableStartPoint_y -= point_num * INTER_LEN_CHANGE;
		}
		else if (zoomWhellChange == WHELL_ZOOM_OUT)
		{
			tableStartPoint_y += point_num * INTER_LEN_CHANGE;
		}
	}

	//restrict the Coordinate axes
	if (tableStartPoint_y > 0)
	{
		tableStartPoint_y = 0;
	}

	if (tableStartPoint_y < End_Point)
	{
		tableStartPoint_y = End_Point;
	}

	Coordinate coor(COOR_MODE_1, Start_Point.x, Start_Point.y, xCoor_Len, yCoor_Len, hdcBuffer);

	coor.StartPaint();

	vector<string>::iterator it_Data = p_opfile->GetInfor_it(0, DATA_NUM);
	coor.DrawCoordinate(
		tableStartPoint_y, 
		nowInterLen,
		p_opfile->GetInfor_Capacity(DATA_NUM),
		VERTICAL_COOR,
		it_Data
		);

	vector<string> BallNum;
	char num[4];
	for (int i = 0; i < 34; i++)
	{
		_itoa_s(i, num, 10);
		BallNum.push_back(num);
	}
	for (int i = 1; i < 17; i++)
	{
		_itoa_s(i, num, 10);
		BallNum.push_back(num);
	}

	vector<string>::iterator it_BallNum = BallNum.begin();

	coor.DrawCoordinate(
		0, 
		20, 
		50,
		HORZION_COOR,
		it_BallNum
		);

	// 对应号画点
	vector<U8>::iterator it_redBallNum = p_opfile->GetInfor_it(0, 0, US_RED_BALL);
	vector<U8>::iterator it_blueBallNum = p_opfile->GetInfor_it(0, 0, BLUE_BALL);
	for (int i = 0; i < 1929; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			int redBallNum = (int)it_redBallNum[i * 6 + j];
			int blueBallNum = (int)it_blueBallNum[i];
			coor.DrawPoint(redBallNum, i, 10, BRUSH_RED, redBallNum);
			coor.DrawPoint(blueBallNum + 33, i, 10, BRUSH_LIGHT_BLUE, blueBallNum);
		}
	}

	coor.EndPaint();
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
	tableAreaRect.bottom = windowAreaRect.bottom - 40;
	tableAreaRect.left = windowAreaRect.left + 50;
	tableAreaRect.right = tableAreaRect.left + 49 * 25;
}

void Analisis::ChangeShowArea(short MouseWhell)
{
	whellMouseLocation = nowMouseLocation;
	if (MouseWhell < 0)	//scroll down to zoom out
	{
		if (newInterLen > 20)
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
		tableSlideFlag_x = FALSE;
		tableSlideFlag_y = FALSE;
		SetCapture(hwnd);	//capture cursor outside window
		break;

	case WM_LBUTTONUP:
		lButtonDownFlag = FALSE;
		tableSlideMove_x = (double)(nowMouseLocation.x - preMouseLocation.x);

		if (abs(tableSlideMove_x) >= 3)
		{
			tableSlideFlag_x = TRUE;
		}

		tableSlideMove_y = (double)(nowMouseLocation.y - preMouseLocation.y);

		if (abs(tableSlideMove_y) >= 3)
		{
			tableSlideFlag_y = TRUE;
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
			mouseMove_y = nowMouseLocation.y - preMouseLocation.y;
			zoomWhellChange = WHELL_ZOOM_NON;
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

	if (tableSlideFlag_x)
	{
		mouseMove_x = (int)tableSlideMove_x;
		InvalidateRect(hwnd, &tableAreaRect, FALSE);
		tableSlideMove_x *= 0.9;
		if (abs(tableSlideMove_x) < 2)
		{
			tableSlideFlag_x = FALSE;
			tableSlideMove_x = 0;
		}
	}

	if (tableSlideFlag_y)
	{
		mouseMove_y = (int)tableSlideMove_y;
		InvalidateRect(hwnd, &tableAreaRect, FALSE);
		tableSlideMove_y *= 0.9;
		if (abs(tableSlideMove_y) < 2)
		{
			tableSlideFlag_y = FALSE;
			tableSlideMove_y = 0;
		}
	}

}

void Analisis::Exit()
{
}
