#pragma once

typedef struct
{
	HDC hdc;
	LONG *xPoint;
	LONG *yPoint;
	int xLen;
	int yLen;
	int xCanvasSize;
	int yCanvasSize;
	int xCanvasPoint;
	int yCanvasPoint;
	int x;
	int y;
}tCOOR_PARAM;

class Coordinate
{
public:
	Coordinate(int x, int y, int xLen, int yLen, HDC hdc);
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
		int yPointNum);

private:
	tCOOR_PARAM tCoor_Param;

	HDC hdcCoorBuffer;
	HBITMAP CoorBitMap;
	HBITMAP PreCoorBitMap;
};

