#include "stdafx.h"
#include "Coordinate.h"

Coordinate::Coordinate(int x, int y, int xLen, int yLen, HDC hdc)
{
	tCoor_Param.xPoint = NULL;
	tCoor_Param.yPoint = NULL;

	tCoor_Param.x = x;
	tCoor_Param.y = y;

	tCoor_Param.xLen = xLen;
	tCoor_Param.yLen = yLen;

	tCoor_Param.hdc = hdc;
}


Coordinate::~Coordinate(void)
{
	if (tCoor_Param.xPoint != NULL)
	{
		free(tCoor_Param.xPoint);
	}
	if (tCoor_Param.yPoint != NULL)
	{
		free(tCoor_Param.yPoint);
	}
}

//Draw Horizon coordinnate
void Coordinate::DrawCoordinate(
	HDC hdcBuffer, 
	int StartLocation, 
	int InterLen,
	int PointCnt, 
	unsigned char CoorType,
	vector<string>::iterator TextString)
{
	HFONT hPreFont, hDataFont;
	PaintFun paint;
	hDataFont = paint.CreateMyFont(hdcBuffer, (LPCTSTR)("Î¢ÈíÑÅºÚ"), 10, 15, 0);
	hPreFont = (HFONT)SelectObject(hdcBuffer, hDataFont);
	HPEN hPrePen = (HPEN)SelectObject(hdcBuffer, GetStockObject(BLACK_PEN));
	SetBkColor(hdcBuffer, BRUSH_WHITE);

	wchar_t *lpcText;
	int valueable_point_cnt = 0;
	unsigned int start_point = 0xFFFF, end_point;

	LONG *total_apt = (LONG*)malloc(sizeof(LONG) * (PointCnt + 1));		//add point 0
	POINT *apt;

	if (CoorType == HORZION_COOR)
	{
		if ((PointCnt * InterLen - tCoor_Param.xLen) < 0)
		{
			tCoor_Param.xLen = PointCnt * InterLen;
		}

		if (StartLocation > 0)
		{
			StartLocation = 0;
		}
		if (StartLocation <  - (PointCnt * InterLen - tCoor_Param.xLen))
		{
			StartLocation =  - (PointCnt * InterLen - tCoor_Param.xLen);
		}

		//calculate point location
		total_apt[0] = StartLocation;

		if (tCoor_Param.xPoint != NULL)
		{
			free(tCoor_Param.xPoint);
		}

		tCoor_Param.xPoint = (LONG *)malloc(sizeof(LONG) * PointCnt);
		tCoor_Param.xLen = tCoor_Param.xLen;

		for (int i = 0; i < PointCnt; ++i)
		{
			total_apt[i] = (int)(i * InterLen) + total_apt[0];
			tCoor_Param.xPoint[i] = total_apt[i] + tCoor_Param.x;

			if (total_apt[i] >= 0 && total_apt[i] < tCoor_Param.xLen)
			{
				valueable_point_cnt++;
				if (start_point == 0xFFFF)
				{
					start_point = i;
				}
				end_point = i;
			}
		}

		apt = (POINT*)malloc(sizeof(POINT) * (valueable_point_cnt * 3 + 2));	//add start and end point

		apt[0].x = tCoor_Param.x;
		apt[0].y = tCoor_Param.y;


		for (int i = 1; i <= valueable_point_cnt; i++)
		{
			apt[i * 3 - 2].x = total_apt[start_point + i - 1] + tCoor_Param.x;
			apt[i * 3 - 2].y = apt[0].y;

			apt[i * 3 - 1].x = apt[i * 3 - 2].x;
			apt[i * 3 - 1].y = apt[0].y - 5;

			apt[i * 3] = apt[i * 3 - 2];

			lpcText = (wchar_t *) malloc(sizeof(wchar_t) *(TextString[start_point + i - 1].length() + 1));
			StringToLPCWSTR(TextString[start_point + i - 1], lpcText);
			TextOut(hdcBuffer, apt[i * 3].x, apt[i * 3].y, lpcText, TextString[start_point + i - 1].length());
			free(lpcText);
		}

		apt[valueable_point_cnt * 3 + 1].x = tCoor_Param.x + tCoor_Param.xLen;
		apt[valueable_point_cnt * 3 + 1].y = tCoor_Param.y;
	}
	else if (CoorType == VERTICAL_COOR)
	{
		if ((PointCnt * InterLen - tCoor_Param.yLen) < 0)
		{
			tCoor_Param.yLen = PointCnt * InterLen;
		}

		if (StartLocation > 0)
		{
			StartLocation = 0;
		}
		if (StartLocation <  - (PointCnt * InterLen - tCoor_Param.yLen))
		{
			StartLocation =  - (PointCnt * InterLen - tCoor_Param.yLen);
		}

		if (tCoor_Param.yPoint != NULL)
		{
			free(tCoor_Param.yPoint);
		}
		tCoor_Param.yPoint = (LONG *)malloc(sizeof(LONG) * PointCnt);
		tCoor_Param.yLen = tCoor_Param.yLen;

		total_apt[0] = - StartLocation;

		int i;
		for (i = 0; i < PointCnt; ++i)
		{
			total_apt[i] = total_apt[0] - (int)(i * InterLen);
			tCoor_Param.yPoint[i] = total_apt[i] + tCoor_Param.y;
			if (total_apt[i] <= 0 && total_apt[i] >= - tCoor_Param.yLen)
			{
				valueable_point_cnt++;
				if (start_point == 0xFFFF)
				{
					start_point = i;
				}
				end_point = i;
			}
		}

		apt = (POINT*)malloc(sizeof(POINT) * (valueable_point_cnt * 3 + 2));	//add start and end point

		apt[0].x = tCoor_Param.x;
		apt[0].y = tCoor_Param.y;

		for (int i = 1; i <= valueable_point_cnt; i++)
		{
			apt[i * 3 - 2].x = apt[0].x;
			apt[i * 3 - 2].y = total_apt[start_point + i - 1] + tCoor_Param.y;

			apt[i * 3 - 1].x = apt[0].x + 5;
			apt[i * 3 - 1].y = apt[i * 3 - 2].y;

			apt[i * 3] = apt[i * 3 - 2];

			lpcText = (wchar_t *) malloc(sizeof(wchar_t) *(TextString[start_point + i - 1].length() + 1));
			StringToLPCWSTR(TextString[start_point + i - 1], lpcText);
			TextOut(hdcBuffer, 
				apt[i * 3].x - (TextString[start_point + i - 1].length() * 10 + 4), 
				apt[i * 3].y - (15/2), 
				lpcText, 
				TextString[start_point + i - 1].length());
			free(lpcText);
		}

		apt[valueable_point_cnt * 3 + 1].x = tCoor_Param.x;
		apt[valueable_point_cnt * 3 + 1].y = tCoor_Param.y - tCoor_Param.yLen;
	}

	Polyline(hdcBuffer, apt, valueable_point_cnt * 3 + 2);
	free(apt);
	free(total_apt);
	SelectObject(hdcBuffer, hPreFont);
	DeleteObject(hDataFont);
	SelectObject(hdcBuffer, hPrePen);
	DeleteObject(GetStockObject(BLACK_PEN));
}


void Coordinate::DrawPoint(
	HDC hdcBuffer,
	int xPointNum,
	int yPointNum)
{

	if (tCoor_Param.xPoint == NULL || tCoor_Param.yPoint == NULL)
	{
		return;
	}

	HDC hdcPointBuffer;
	HBITMAP *bitmapBuff = new HBITMAP;
	HBITMAP PreBitmap;
	PaintFun paintfun;

	HPEN hPrePen = (HPEN)SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	*bitmapBuff =  CreateCompatibleBitmap(tCoor_Param.hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	hdcPointBuffer = CreateCompatibleDC(tCoor_Param.hdc);
	PreBitmap = (HBITMAP)SelectObject(hdcPointBuffer, *bitmapBuff);

	PatBlt(hdcPointBuffer, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), WHITENESS);

	int Left = tCoor_Param.xPoint[xPointNum] - 10;
	int Top = tCoor_Param.yPoint[yPointNum] - 10;
	int Right = tCoor_Param.xPoint[xPointNum] + 10;
	int Buttom = tCoor_Param.yPoint[yPointNum] + 10;

	HBRUSH hBrushBlue = CreateSolidBrush(BRUSH_DEEP_BLUE);
	HBRUSH hPreBrush = (HBRUSH)SelectObject(hdcPointBuffer, hBrushBlue);

	Ellipse(hdcPointBuffer, Left, Top, Right, Buttom);

	BitBlt(hdcBuffer, tCoor_Param.x, tCoor_Param.y - tCoor_Param.yLen, tCoor_Param.xLen, tCoor_Param.yLen, 
		hdcPointBuffer, tCoor_Param.x, tCoor_Param.y - tCoor_Param.yLen, SRCAND);

	SelectObject(hdcBuffer, hPreBrush);
	DeleteObject(hBrushBlue);
	SelectObject(hdcBuffer, hPrePen);
	DeleteObject(GetStockObject(NULL_PEN));

	SelectObject(hdcPointBuffer, PreBitmap);
	DeleteObject(*bitmapBuff);
	DeleteDC(hdcPointBuffer);
}

