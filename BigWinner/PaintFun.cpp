#include "PaintFun.h"


PaintFun::PaintFun(void)
{
}


PaintFun::~PaintFun(void)
{
}


void PaintFun::DrawRect(HDC *hdcBuffer, COLORREF color, RECT rect, COLORREF pen)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_SOLID, 1, pen);
	SelectObject(*hdcBuffer, hPen);
	SelectObject(*hdcBuffer, hBrush);
	Rectangle(*hdcBuffer, 
		rect.left, 
		rect.top, 
		rect.right, 
		rect.bottom
		);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void PaintFun::DrawFrame(HDC *hdcBuffer, RECT rect)
{
	rect.left += 3;
	rect.top += 3;
	rect.right += 3; 
	rect.bottom += 3;
	DrawRect(hdcBuffer, BRUSH_BLACK, rect, BRUSH_BLACK);
	rect.left -= 4;
	rect.top -= 4;
	rect.right -= 2; 
	rect.bottom -= 2;
	DrawRect(hdcBuffer, BRUSH_GRAY, rect, BRUSH_GRAY);
	rect.left += 1;
	rect.top += 1;
	rect.right -= 1; 
	rect.bottom -= 1;
	DrawRect(hdcBuffer, BRUSH_WHITE, rect, BRUSH_WHITE);
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
	*hDataFont = CreateMyFont(*hdcBuffer, (LPCTSTR)("����"), 10, 10, -60);
	SelectObject(*hdcBuffer, hDataFont);
	SelectObject(*hdcBuffer, GetStockObject(BLACK_PEN));
	SetBkColor(*hdcBuffer, BRUSH_WHITE);

	wchar_t *lpcText;
	unsigned char TextLen = TextString->length();
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
			TextOut(*hdcBuffer, apt[i * 3].x, apt[i * 3].y, lpcText, TextLen);
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
		height,      //������߼��߶�
		width,       //�߼�ƽ���ַ����
		angle,       //��ˮƽ�ߵĽǶ�
		0,           //���߷�λ�Ƕ�
		FW_REGULAR,  //���Σ�����
		FALSE,       //���Σ�б��
		FALSE,       //���Σ��»���
		FALSE,       //���Σ�����
		GB2312_CHARSET,          //�ַ���
		OUT_DEFAULT_PRECIS,      //�������
		CLIP_DEFAULT_PRECIS,     //���ؾ���
		PROOF_QUALITY,           //���Ʒ��
		FIXED_PITCH | FF_MODERN, //��б��
		face                     //����
		); 
	return hFont;
}

HWND PaintFun::CreateButton(int x, int y, int width, int length, HWND parent_hwnd, HINSTANCE hInstance, int ID)
{
	return CreateWindow(TEXT("button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, length, parent_hwnd, (HMENU)ID, hInstance, NULL);
}

void PaintFun::DrawButton(LPDRAWITEMSTRUCT pdis, TCHAR *text, int textnum)
{
	TEXTMETRIC tm;
	GetTextMetrics(pdis->hDC, &tm);
	int xText = (pdis->rcItem.right - pdis->rcItem.left) / 2 - (((tm.tmAveCharWidth + tm.tmMaxCharWidth) / 2) * textnum) / 2;
	int yText = (pdis->rcItem.bottom - pdis->rcItem.top) / 2 - tm.tmHeight / 2;
	DrawRect(&pdis->hDC, BRUSH_LIGHT_GRAY, pdis->rcItem, BRUSH_DEEP_GRAY);
	SetBkColor(pdis->hDC, BRUSH_LIGHT_GRAY);
	TextOut(pdis->hDC, xText, yText, text, 4);

	if (pdis->itemState & ODS_SELECTED)
	{
		DrawRect(&pdis->hDC, BRUSH_DEEP_GRAY, pdis->rcItem, BRUSH_DEEP_GRAY);
		SetBkColor(pdis->hDC, BRUSH_DEEP_GRAY);
		TextOut(pdis->hDC, xText, yText, text, 4);
	}

// 	if (pdis->itemState & ODS_FOCUS)  
// 	{
// 		int cx = pdis->rcItem.right  - pdis->rcItem.left ;  
// 		int cy = pdis->rcItem.bottom - pdis->rcItem.top  ;  
// 
// 		pdis->rcItem.left  += cx / 16;  
// 		pdis->rcItem.top   +=  cy / 16;  
// 		pdis->rcItem.right -=  cx / 16;  
// 		pdis->rcItem.bottom-=  cy / 16;  
// 
// 		DrawFocusRect  (pdis->hDC, &pdis->rcItem) ;  
// 	}  
}
