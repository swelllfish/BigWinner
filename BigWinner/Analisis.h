#pragma once

#include "stdafx.h"

class Analisis
{
public:
	Analisis(void);
	~Analisis(void);

	void GetFilePoint(OpFile *opfile);
	void ShowTable(HDC *hdc, OpFile *opfile);
	void DrawBackGround(HDC *hdc);
	void SetWorkSpaceArea(int x, int y);
	void ChangeShowArea(short MouseWhell);
	void MouseAction(HWND hwnd, short x, short y, unsigned short act_type);
	void DrawCoordinate(HDC *hdc);
	void InvalidateArea(HWND hwnd);
	void Exit();

private:
	PaintFun paintfun;
	OpFile *p_opfile;
	RECT  WorkRect;		//the Window size
	RECT  ShowRect;		//Table size
	int   InterLen;		//Old Horrizon coordinate point cnt
	int   NewInterLen;	//Horrizon coordinate point cnt
	bool  LButton_Down_Flag;
	bool  Slide_Flag;
	bool  Zoom_Change;
	POINT Pre_Mouse_Location;
	POINT Now_Mouse_Location;
	int   Mouse_xMove;	//Table move follow the cursor
	double Slide_xMove;  //Table Slide move
	int   Point_Start_Location;	//First Point Location
};

