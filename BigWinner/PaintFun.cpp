#include "PaintFun.h"


PaintFun::PaintFun(void)
{
}


PaintFun::~PaintFun(void)
{
}


void PaintFun::DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect, char pen)
{
	SelectObject(*hdcBuffer, GetStockObject(pen));
	SelectObject(*hdcBuffer, CreateSolidBrush(color));
	Rectangle(*hdcBuffer, 
		rect.left, 
		rect.top, 
		rect.right, 
		rect.bottom
		);
}

void PaintFun::DrawFrame(HDC *hdcBuffer, RECT rect)
{
	rect.left += 3;
	rect.top += 3;
	rect.right += 3; 
	rect.bottom += 3;
	DrawRect(hdcBuffer, BRUSH_BLACK, rect, NULL_PEN);
	rect.left -= 4;
	rect.top -= 4;
	rect.right -= 2; 
	rect.bottom -= 2;
	DrawRect(hdcBuffer, BRUSH_GRAY, rect, NULL_PEN);
	rect.left += 1;
	rect.top += 1;
	rect.right -= 1; 
	rect.bottom -= 1;
	DrawRect(hdcBuffer, BRUSH_WHITE, rect, NULL_PEN);
}

//Draw Horizon coordinnate
//int start means how many percentage of the first point location in the first interval, value number is between 0 to 100
void PaintFun::DrawCoordinate(HDC *hdcBuffer, int xlocation, int ylocation, int len, int cnt, int start, unsigned char coortype)
{
	SelectObject(*hdcBuffer, GetStockObject(BLACK_PEN));
	MoveToEx(*hdcBuffer, xlocation, ylocation, NULL);
	int interlen = len / cnt;
	int startpoint = (int)((double)start * (double)interlen / 100);

	POINT *apt;

	int TotalPointCnt = 3 * cnt + 2;
	apt = (POINT *)malloc(sizeof(POINT) * (TotalPointCnt)); //start point and end point need to be special handled


	if (coortype == HORZION_COOR)
	{
		//start point is not at the 0 point
		apt[0].x = xlocation;
		apt[0].y = ylocation;

		apt[1].x = xlocation + (int)startpoint;
		apt[1].y = ylocation;
		int i;
		//start point and end point need to be special handled
		for (i = 2; i < TotalPointCnt - 1; i += 3)
		{
			apt[i].x = apt[i-1].x;
			apt[i].y = apt[i-1].y - 5;

			apt[i + 1].x = apt[i - 1].x;
			apt[i + 1].y = apt[i - 1].y;

			apt[i + 2].x = apt[i - 1].x + interlen;
			apt[i + 2].y = apt[i - 1].y;
		}
		apt[TotalPointCnt - 1].x = apt[TotalPointCnt - 2].x + interlen - startpoint;
		apt[TotalPointCnt - 1].y = apt[TotalPointCnt - 2].y;
	}
	else if (coortype == VERTICAL_COOR)
	{
		//start point is not at the 0 point
		apt[0].x = xlocation;
		apt[0].y = ylocation;

		apt[1].x = xlocation;
		apt[1].y = ylocation - (int)startpoint;
		int i;
		//start point and end point need to be special handled
		for (i = 2; i < TotalPointCnt - 1; i += 3)
		{
			apt[i].x = apt[i-1].x + 5;
			apt[i].y = apt[i-1].y;

			apt[i + 1].x = apt[i - 1].x;
			apt[i + 1].y = apt[i - 1].y;

			apt[i + 2].x = apt[i - 1].x;
			apt[i + 2].y = apt[i - 1].y - interlen;
		}
		apt[TotalPointCnt - 1].x = apt[TotalPointCnt - 2].x;
		apt[TotalPointCnt - 1].y = apt[TotalPointCnt - 2].y - interlen + startpoint;
	}

	Polyline(*hdcBuffer, apt, TotalPointCnt);
	free(apt);
}

