#include "PaintFun.h"


PaintFun::PaintFun(void)
{
}


PaintFun::~PaintFun(void)
{
}


void PaintFun::DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect, char pen)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(*hdcBuffer, GetStockObject(pen));
	SelectObject(*hdcBuffer, hBrush);
	Rectangle(*hdcBuffer, 
		rect.left, 
		rect.top, 
		rect.right, 
		rect.bottom
		);
	DeleteObject(hBrush);
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
//float start means how many percentage of the first point location in the first interval, value number is between 0 to len
void PaintFun::DrawCoordinate(
	HDC *hdcBuffer, 
	int xlocation, 
	int ylocation, 
	int start, 
	int len,
	int inter_len,
	int total_cnt, 
	unsigned char coortype,
	vector<string>::iterator TextString)
{
	HFONT *hDataFont = new HFONT;
	*hDataFont = CreateMyFont(*hdcBuffer, (LPCTSTR)("宋体"), 10, 10, -60);
	SelectObject(*hdcBuffer, hDataFont);
	SelectObject(*hdcBuffer, GetStockObject(BLACK_PEN));
	SetBkColor(*hdcBuffer, BRUSH_WHITE);

	wchar_t *lpcText;
	unsigned char TextLen = TextString->length();
	int valueable_point_cnt = 0;
	unsigned int start_point = 0xFFFF, end_point;

	LONG *total_apt = (LONG*)malloc(sizeof(LONG) * (total_cnt + 1));		//add point 0
	POINT *apt;

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
		total_apt[0] = xlocation + start;
		if (total_apt[0] == xlocation)
		{
			start_point = 0;
			valueable_point_cnt++;
		}

		for (int i = 1; i < total_cnt + 1; ++i)
		{
			total_apt[i] = (int)(i * inter_len) + total_apt[0];
			if (total_apt[i] >= xlocation && total_apt[i] < xlocation + len)
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
			apt[i * 3 - 2].x = total_apt[start_point + i - 1];
			apt[i * 3 - 2].y = apt[0].y;

			apt[i * 3 - 1].x = apt[i * 3 - 2].x;
			apt[i * 3 - 1].y = apt[0].y - 5;

			apt[i * 3] = apt[i * 3 - 2];

			lpcText = (wchar_t *) malloc(sizeof(wchar_t) *(TextString[start_point + i - 1].length() + 1));
			StringToLPCWSTR(TextString[start_point + i - 1], lpcText);
			TextOut(*hdcBuffer, apt[i * 3].x, apt[i * 3].y, lpcText, TextLen);
			free(lpcText);
		}

		apt[valueable_point_cnt * 3 + 1].x = xlocation + len;
		apt[valueable_point_cnt * 3 + 1].y = ylocation;
	}
	else if (coortype == VERTICAL_COOR)
	{
		total_apt[0] = ylocation - start;
		if (total_apt[0] == xlocation)
		{
			start_point = 0;
		}

		int i;
		for (i = 1; i < total_cnt + 1; ++i)
		{
			total_apt[i] = total_apt[0] - (int)(i * inter_len);
			if (total_apt[i] <= ylocation && total_apt[i] > ylocation - len)
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
			apt[i * 3 - 2].y = total_apt[start_point + i - 1];

			apt[i * 3 - 1].x = apt[0].x + 5;
			apt[i * 3 - 1].y = apt[i * 3 - 2].y;

			apt[i * 3] = apt[i * 3 - 2];
		}

		apt[valueable_point_cnt * 3 + 1].x = xlocation;
		apt[valueable_point_cnt * 3 + 1].y = ylocation - len;
	}

	Polyline(*hdcBuffer, apt, valueable_point_cnt * 3 + 2);
	free(apt);
	free(total_apt);
	DeleteObject(hDataFont);
	DeleteObject(GetStockObject(BLACK_PEN));
	delete(hDataFont);
}

HFONT PaintFun::CreateMyFont(HDC hdc, LPCTSTR face, int width, int height, int angle)
{
	HFONT hFont;
	hFont = CreateFont(
		height,      //字体的逻辑高度
		width,       //逻辑平均字符宽度
		angle,       //与水平线的角度
		0,           //基线方位角度
		FW_REGULAR,  //字形：常规
		FALSE,       //字形：斜体
		FALSE,       //字形：下划线
		FALSE,       //字形：粗体
		GB2312_CHARSET,          //字符集
		OUT_DEFAULT_PRECIS,      //输出精度
		CLIP_DEFAULT_PRECIS,     //剪截精度
		PROOF_QUALITY,           //输出品质
		FIXED_PITCH | FF_MODERN, //倾斜度
		face                     //字体
		); 
	return hFont;
}

