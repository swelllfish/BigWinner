#pragma once

typedef struct
{
	LONG *x;
	LONG *y;
	int xLen;
	int yLen;
}tCOOR_PARAM;

class Coordinate
{
public:
	Coordinate(void);
	~Coordinate(void);

	tCOOR_PARAM tCoor_Param;

	void DrawCoordinate(
		HDC hdcBuffer, 
		int xLocation, 
		int yLocation, 
		int StartLocation, 
		int CoorLen,
		int InterLen,
		int PointCnt, 
		unsigned char CoorType,
		vector<string>::iterator TextString);

	void DrawPoint(
		HDC hdcBuffer,
		int xPointNum,
		int yPointNum);
};

