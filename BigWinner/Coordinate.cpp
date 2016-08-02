#include "stdafx.h"
#include "Coordinate.h"

Coordinate::Coordinate(void)
{
	tCoor_Param.x = NULL;
	tCoor_Param.y = NULL;
}


Coordinate::~Coordinate(void)
{
	if (tCoor_Param.x != NULL)
	{
		free(tCoor_Param.x);
	}
	if (tCoor_Param.y != NULL)
	{
		free(tCoor_Param.y);
	}
}

//Draw Horizon coordinnate
void Coordinate::DrawCoordinate(
	HDC hdcBuffer, 
	int xLocation, 
	int yLocation, 
	int StartLocation, 
	int CoorLen,
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

	if ((PointCnt * InterLen - CoorLen) < 0)
	{
		CoorLen = PointCnt * InterLen;
	}

	if (StartLocation > 0)
	{
		StartLocation = 0;
	}
	if (StartLocation <  - (PointCnt * InterLen - CoorLen))
	{
		StartLocation =  - (PointCnt * InterLen - CoorLen);
	}

	if (CoorType == HORZION_COOR)
	{
		//calculate point location
		total_apt[0] = StartLocation;

		if (tCoor_Param.x != NULL)
		{
			free(tCoor_Param.x);
		}

		tCoor_Param.x = (LONG *)malloc(sizeof(LONG) * PointCnt);
		tCoor_Param.xLen = CoorLen;

		for (int i = 0; i < PointCnt; ++i)
		{
			total_apt[i] = (int)(i * InterLen) + total_apt[0];
			tCoor_Param.x[i] = total_apt[i] + xLocation;

			if (total_apt[i] >= 0 && total_apt[i] < CoorLen)
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

		apt[0].x = xLocation;
		apt[0].y = yLocation;


		for (int i = 1; i <= valueable_point_cnt; i++)
		{
			apt[i * 3 - 2].x = total_apt[start_point + i - 1] + xLocation;
			apt[i * 3 - 2].y = apt[0].y;

			apt[i * 3 - 1].x = apt[i * 3 - 2].x;
			apt[i * 3 - 1].y = apt[0].y - 5;

			apt[i * 3] = apt[i * 3 - 2];

			lpcText = (wchar_t *) malloc(sizeof(wchar_t) *(TextString[start_point + i - 1].length() + 1));
			StringToLPCWSTR(TextString[start_point + i - 1], lpcText);
			TextOut(hdcBuffer, apt[i * 3].x, apt[i * 3].y, lpcText, TextString[start_point + i - 1].length());
			free(lpcText);
		}

		apt[valueable_point_cnt * 3 + 1].x = xLocation + CoorLen;
		apt[valueable_point_cnt * 3 + 1].y = yLocation;
	}
	else if (CoorType == VERTICAL_COOR)
	{
		if (tCoor_Param.y != NULL)
		{
			free(tCoor_Param.y);
		}
		tCoor_Param.y = (LONG *)malloc(sizeof(LONG) * PointCnt);
		tCoor_Param.yLen = CoorLen;

		total_apt[0] = - StartLocation;

		int i;
		for (i = 0; i < PointCnt; ++i)
		{
			total_apt[i] = total_apt[0] - (int)(i * InterLen);
			tCoor_Param.y[i] = total_apt[i] + yLocation;
			if (total_apt[i] <= 0 && total_apt[i] >= - CoorLen)
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

		apt[0].x = xLocation;
		apt[0].y = yLocation;

		for (int i = 1; i <= valueable_point_cnt; i++)
		{
			apt[i * 3 - 2].x = apt[0].x;
			apt[i * 3 - 2].y = total_apt[start_point + i - 1] + yLocation;

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

		apt[valueable_point_cnt * 3 + 1].x = xLocation;
		apt[valueable_point_cnt * 3 + 1].y = yLocation - CoorLen;
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
	if (tCoor_Param.x == NULL || tCoor_Param.y == NULL)
	{
		return;
	}

	int Left = tCoor_Param.x[xPointNum] - 5;
	int Top = tCoor_Param.y[yPointNum] - 5;
	int Right = tCoor_Param.x[xPointNum] + 5;
	int Buttom = tCoor_Param.y[yPointNum] + 5;

	HPEN hPrePen = (HPEN)SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	HBRUSH hBrush = CreateSolidBrush(BRUSH_DEEP_GRAY);
	HBRUSH hPreBrush = (HBRUSH)SelectObject(hdcBuffer, hBrush);

	Ellipse(hdcBuffer, Left, Top, Right, Buttom);

	SelectObject(hdcBuffer, hPreBrush);
	DeleteObject(hBrush);
	SelectObject(hdcBuffer, hPrePen);
	DeleteObject(GetStockObject(NULL_PEN));
}

