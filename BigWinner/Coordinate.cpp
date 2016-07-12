#include "stdafx.h"
#include "Coordinate.h"

Coordinate::Coordinate(void)
{
}


Coordinate::~Coordinate(void)
{
}

//Draw Horizon coordinnate
//int start means how many percentage of the first point location in the first interval, value number is between 0 to len
void Coordinate::DrawCoordinate(
	HDC hdcBuffer, 
	int xlocation, 
	int ylocation, 
	int start, 
	int len,
	int inter_len,
	int total_cnt, 
	unsigned char coortype,
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

	LONG *total_apt = (LONG*)malloc(sizeof(LONG) * (total_cnt + 1));		//add point 0
	POINT *apt;

	if ((total_cnt * inter_len - len) < 0)
	{
		len = total_cnt * inter_len;
	}

	if (start > 0)
	{
		start = 0;
	}
	if (start <  - (total_cnt * inter_len - len))
	{
		start =  - (total_cnt * inter_len - len);
	}

	if (coortype == HORZION_COOR)
	{
		//calculate point location
		total_apt[0] = start;
		if (total_apt[0] == 0)
		{
			start_point = 0;
			valueable_point_cnt++;
		}

		for (int i = 1; i < total_cnt + 1; ++i)
		{
			total_apt[i] = (int)(i * inter_len) + total_apt[0];
			if (total_apt[i] >= 0 && total_apt[i] < len)
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

		apt[0].x = xlocation;
		apt[0].y = ylocation;


		for (int i = 1; i <= valueable_point_cnt; i++)
		{
			apt[i * 3 - 2].x = total_apt[start_point + i - 1] + xlocation;
			apt[i * 3 - 2].y = apt[0].y;

			apt[i * 3 - 1].x = apt[i * 3 - 2].x;
			apt[i * 3 - 1].y = apt[0].y - 5;

			apt[i * 3] = apt[i * 3 - 2];

			lpcText = (wchar_t *) malloc(sizeof(wchar_t) *(TextString[start_point + i - 1].length() + 1));
			StringToLPCWSTR(TextString[start_point + i - 1], lpcText);
			TextOut(hdcBuffer, apt[i * 3].x, apt[i * 3].y, lpcText, TextString[start_point + i - 1].length());
			free(lpcText);
		}

		apt[valueable_point_cnt * 3 + 1].x = xlocation + len;
		apt[valueable_point_cnt * 3 + 1].y = ylocation;
	}
	else if (coortype == VERTICAL_COOR)
	{
		total_apt[0] = - start;
		if (total_apt[0] == ylocation)
		{
			start_point = 0;
		}

		int i;
		for (i = 1; i < total_cnt + 1; ++i)
		{
			total_apt[i] = total_apt[0] - (int)(i * inter_len);
			if (total_apt[i] <= 0 && total_apt[i] >= - len)
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

		apt[0].x = xlocation;
		apt[0].y = ylocation;

		for (int i = 1; i <= valueable_point_cnt; i++)
		{
			apt[i * 3 - 2].x = apt[0].x;
			apt[i * 3 - 2].y = total_apt[start_point + i - 1] + ylocation;

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

		apt[valueable_point_cnt * 3 + 1].x = xlocation;
		apt[valueable_point_cnt * 3 + 1].y = ylocation - len;
	}

	Polyline(hdcBuffer, apt, valueable_point_cnt * 3 + 2);
	free(apt);
	free(total_apt);
	SelectObject(hdcBuffer, hPreFont);
	DeleteObject(hDataFont);
	SelectObject(hdcBuffer, hPrePen);
	DeleteObject(GetStockObject(BLACK_PEN));
}
