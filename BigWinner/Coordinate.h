#pragma once

typedef struct
{
	HDC hdc;
	LONG *xPoint;
	LONG *yPoint;
	int xLen;
	int yLen;
	int x;
	int y;
}tCOOR_PARAM;

class Coordinate
{
public:
	Coordinate(int x, int y, int xLen, int yLen, HDC hdc);
	~Coordinate(void);

	tCOOR_PARAM tCoor_Param;

	void DrawCoordinate(
		HDC hdcBuffer, 
		int StartLocation, 
		int InterLen,
		int PointCnt, 
		unsigned char CoorType,
		vector<string>::iterator TextString);

	void DrawPoint(
		HDC hdcBuffer,
		int xPointNum,
		int yPointNum);
};

