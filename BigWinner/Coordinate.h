#pragma once


#define COOR_MODE_0 0
#define COOR_MODE_1 1
#define COOR_MODE_2 2
#define COOR_MODE_3 3
//������4��Mode
//Mode0 - ԭ�������½ǣ�x���������죬y����������
//Mode1 - ԭ�������Ͻǣ�x���������죬y����������
//Mode2 - ԭ�������½ǣ�x���������죬y����������
//Mode3 - ԭ�������Ͻǣ�x���������죬y����������
typedef struct
{
	HDC hdc;
	unsigned char bMode;    
	LONG *xPoint;
	LONG *yPoint;
	int xLen;
	int yLen;
	int x;
	int y;
}tCOOR_PARAM;

typedef struct  
{
	HDC hdcBuffer;
	HBITMAP bitmap;
	HBITMAP Prebitmap;
}tHDC_PARAM;

class Coordinate
{
public:
	Coordinate(unsigned char bMode, int x,  int y, int xLen, int yLen, HDC hdc);
	~Coordinate(void);

	void StartPaint();

	void EndPaint();

	void DrawCoordinate(
		int StartLocation, 
		int InterLen,
		int PointCnt, 
		unsigned char CoorType,
		vector<string>::iterator TextString);

	void DrawPoint(
		int xPointNum,
		int yPointNum,
		int Radium, 
		COLORREF color,
		int showNumber);

private:
	tCOOR_PARAM tCoor_Param;
	
// 	tHDC_PARAM hdc_coor;
	tHDC_PARAM hdc_point;
};

