#pragma once

#include "stdafx.h"

class Analisis : public PaintFun
{
public:
	Analisis(void);
	~Analisis(void);

	void ShowTable(HDC *hdc, OpFile *opfile);
	void DrawBackGround(HDC *hdc);
	void SetWorkSpaceArea(int x, int y);
	void ChangeShowArea(short MouseWhell);
	void InvalidateArea(HWND hwnd);
	void Exit();

private:
	RECT WorkRect;
	RECT ShowRect;
	BOOL MouseWhellFlag;
	short SizeRate;
	int  PointCnt;
	int  NewPointCnt;
};

