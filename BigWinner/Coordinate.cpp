#include "stdafx.h"
#include "Coordinate.h"

Coordinate::Coordinate(int x, int y, int xLen, int yLen, HDC hdc)
{
	//���ڱ��������ݵ�ָ��
	tCoor_Param.xPoint = NULL;  
	tCoor_Param.yPoint = NULL;

	//�����������껭�����е����
	tCoor_Param.x = x;         
	tCoor_Param.y = y;

	//�����᳤��
	tCoor_Param.xLen = xLen;    
	tCoor_Param.yLen = yLen;

// 	//���껭���ĳ���
// 	tCoor_Param.xCanvasSize = xLen + 90;  
// 	tCoor_Param.yCanvasSize = yLen + 40;
// 
// 	//���������ʱӦ��ʹ�õ����λ��
// 	tCoor_Param.xCanvasPoint = x - 80;
// 	tCoor_Param.yCanvasPoint = y - yLen - 10;

	tCoor_Param.hdc = hdc;
}


Coordinate::~Coordinate(void)
{
	//�ͷŵ�����ݣ�����ָ��ָ��NULL
	if (tCoor_Param.xPoint != NULL)
	{
		free(tCoor_Param.xPoint);
		tCoor_Param.xPoint = NULL;
	}
	if (tCoor_Param.yPoint != NULL)
	{
		free(tCoor_Param.yPoint);
		tCoor_Param.yPoint = NULL;
	}
}

void Coordinate::StartPaint()
{
	//���������ử��
// 	hdc_coor.bitmap =  CreateCompatibleBitmap(tCoor_Param.hdc, tCoor_Param.xCanvasSize, tCoor_Param.yCanvasSize);
// 	hdc_coor.hdcBuffer = CreateCompatibleDC(tCoor_Param.hdc);
// 	hdc_coor.Prebitmap = (HBITMAP)SelectObject(hdc_coor.hdcBuffer, hdc_coor.bitmap);
// 
// 	PatBlt(hdc_coor.hdcBuffer, 0, 0, tCoor_Param.xCanvasSize, tCoor_Param.yCanvasSize, WHITENESS);

	//�����Ļ���
	hdc_point.bitmap = CreateCompatibleBitmap(tCoor_Param.hdc, tCoor_Param.xLen, tCoor_Param.yLen);
	hdc_point.hdcBuffer = CreateCompatibleDC(tCoor_Param.hdc);
	hdc_point.Prebitmap = (HBITMAP)SelectObject(hdc_point.hdcBuffer, hdc_point.bitmap);

	PatBlt(hdc_point.hdcBuffer, 0, 0, tCoor_Param.xLen, tCoor_Param.yLen, WHITENESS);
}

void Coordinate::EndPaint()
{
	//�ѵ�Ļ������������ử����
	BitBlt(tCoor_Param.hdc, tCoor_Param.x, tCoor_Param.y - tCoor_Param.yLen, tCoor_Param.xLen, tCoor_Param.yLen, 
		hdc_point.hdcBuffer, 0, 0, SRCAND);

	SelectObject(hdc_point.hdcBuffer, hdc_point.Prebitmap);
	DeleteObject(hdc_point.bitmap);
	DeleteDC(hdc_point.hdcBuffer);

	//�������ử������Ŀ�껭����
// 	BitBlt(tCoor_Param.hdc, tCoor_Param.xCanvasPoint, tCoor_Param.yCanvasPoint, tCoor_Param.xCanvasSize, tCoor_Param.yCanvasSize, 
// 		hdc_coor.hdcBuffer, 0, 0, SRCAND);
// 
// 	SelectObject(hdc_coor.hdcBuffer, hdc_coor.Prebitmap);
// 	DeleteObject(hdc_coor.bitmap);
// 	DeleteDC(hdc_coor.hdcBuffer);
}

//Draw coordinnate
void Coordinate::DrawCoordinate(
	int StartLocation, 
	int InterLen,
	int PointCnt, 
	unsigned char CoorType,
	vector<string>::iterator TextString)
{
	HFONT hPreFont, hDataFont;
	PaintFun paint;
	hDataFont = paint.CreateMyFont((LPCTSTR)("΢���ź�"), 10, 15, 0);
	hPreFont = (HFONT)SelectObject(tCoor_Param.hdc, hDataFont);
	HPEN hPrePen = (HPEN)SelectObject(tCoor_Param.hdc, GetStockObject(BLACK_PEN));
	SetBkColor(tCoor_Param.hdc, BRUSH_WHITE);

	wchar_t *lpcText;
	int valueable_point_cnt = 0;
	unsigned int start_point = 0xFFFF, end_point;

	LONG *total_apt = (LONG*)malloc(sizeof(LONG) * (PointCnt + 1));		//add point 0
	POINT *apt;

	if (CoorType == HORZION_COOR)
	{
		//�������ĵ�*������Ȳ�������������᳤�ȣ��������᳤��ȡ��*�����ֵ
		if ((PointCnt * InterLen - tCoor_Param.xLen) < 0)
		{
			tCoor_Param.xLen = PointCnt * InterLen;
		}

		//0 < x < Coor_Len�Ŀռ�������ʾ��Ӧ�����꣬���StartLoactionһ����С�ڵ���0��
		if (StartLocation > 0)
		{
			StartLocation = 0;
		}
		//StartLocation����̫С����ֹ�޷�����ʾ����ʾ������������
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

		//������Ч�ĵ�����������ѵ�����ݴ���tCoor_Param.x/yPoint���У���֮�󻭵㵱��ʹ��
		for (int i = 0; i < PointCnt; ++i)
		{
			total_apt[i] = (int)(i * InterLen) + total_apt[0];
			tCoor_Param.xPoint[i] = total_apt[i];

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
			TextOut(tCoor_Param.hdc, apt[i * 3].x, apt[i * 3].y, lpcText, TextString[start_point + i - 1].length());
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

		total_apt[0] = - StartLocation;

		int i;
		for (i = 0; i < PointCnt; ++i)
		{
			total_apt[i] = total_apt[0] - (int)(i * InterLen);
			tCoor_Param.yPoint[i] = total_apt[i] + tCoor_Param.yLen;
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
			TextOut(tCoor_Param.hdc, 
				apt[i * 3].x - (TextString[start_point + i - 1].length() * 10 + 4), 
				apt[i * 3].y - (15/2), 
				lpcText, 
				TextString[start_point + i - 1].length());
			free(lpcText);
		}

		apt[valueable_point_cnt * 3 + 1].x = tCoor_Param.x;
		apt[valueable_point_cnt * 3 + 1].y = tCoor_Param.y - tCoor_Param.yLen;
	}

	Polyline(tCoor_Param.hdc, apt, valueable_point_cnt * 3 + 2);
	free(apt);
	free(total_apt);
	SelectObject(tCoor_Param.hdc, hPreFont);
	DeleteObject(hDataFont);
	SelectObject(tCoor_Param.hdc, hPrePen);
	DeleteObject(GetStockObject(BLACK_PEN));
}


void Coordinate::DrawPoint(
	int xPointNum,
	int yPointNum)
{

	if (tCoor_Param.xPoint == NULL || tCoor_Param.yPoint == NULL)
	{
		return;
	}


	int Left = tCoor_Param.xPoint[xPointNum] - 12;
	int Top = tCoor_Param.yPoint[yPointNum] - 12;
	int Right = tCoor_Param.xPoint[xPointNum] + 12;
	int Buttom = tCoor_Param.yPoint[yPointNum] + 12;

	//���������ʾ��Χ�򲻻�
	if (Left > tCoor_Param.xLen
		|| Right < 0
		|| Buttom < 0
		|| Top > tCoor_Param.yLen)
	{
		return;
	}

	PaintFun paintfun;
	HPEN hPrePen = (HPEN)SelectObject(hdc_point.hdcBuffer, GetStockObject(NULL_PEN));

	HBRUSH hBrushBlue = CreateSolidBrush(BRUSH_LIGHT_BLUE);
	HBRUSH hPreBrush = (HBRUSH)SelectObject(hdc_point.hdcBuffer, hBrushBlue);
	SetBkColor(hdc_point.hdcBuffer, BRUSH_LIGHT_BLUE);

	Ellipse(hdc_point.hdcBuffer, Left, Top, Right, Buttom);

	wchar_t Number[3];
	swprintf_s(Number, L"%d", yPointNum);
	PaintFun paint;
	HFONT hDataFont = paint.CreateMyFont((LPCTSTR)("΢���ź�"), 8, 10, 0);
	HFONT hPreFont = (HFONT)SelectObject(hdc_point.hdcBuffer, hDataFont);

	if (yPointNum < 10)
	{
		TextOut(hdc_point.hdcBuffer, Left + 8, Top + 5, Number, 1);
	}
	else
	{
		TextOut(hdc_point.hdcBuffer, Left + 4, Top + 5, Number, 2);
	}

	SelectObject(hdc_point.hdcBuffer, hPreFont);
	DeleteObject(hDataFont);

	SelectObject(hdc_point.hdcBuffer, hPreBrush);
	DeleteObject(hBrushBlue);
	SelectObject(hdc_point.hdcBuffer, hPrePen);
	DeleteObject(GetStockObject(NULL_PEN));
}

