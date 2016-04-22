#pragma once

#include "stdafx.h"

class Analisis : public PaintFun
{
public:
	Analisis(void);
	~Analisis(void);

	void GetFilePoint(OpFile *opfile);
	void ShowTable(HDC *hdc, OpFile *opfile);
	void DrawBackGround(HDC *hdc);
	void SetWorkSpaceArea(int x, int y);
	void ChangeShowArea(short MouseWhell);
	void MouseAction(HWND hwnd, int x, int y, unsigned short act_type);
	void DrawCoordinate(HDC *hdc);
	void InvalidateArea(HWND hwnd);
	void Exit();

private:
	OpFile *p_opfile;
	RECT  WorkRect;		//the Window size
	RECT  ShowRect;		//Table size
	int   PointCnt;		//Old Horrizon coordinate point cnt
	int   NewPointCnt;	//Horrizon coordinate point cnt
	bool  LButton_Down;
	POINT Pre_Mouse_Location;
	POINT Now_Mouse_Location;
	int   Mouse_xMove;	//Table move follow the cursor
	int   Point_Start_Location;	//First Point Location
};

