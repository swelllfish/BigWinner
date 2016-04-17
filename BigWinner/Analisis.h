#pragma once

#include "stdafx.h"

class Analisis : public PaintFun
{
public:
	Analisis(void);
	~Analisis(void);

	void ShowTable(HDC hdc, OpFile *opfile);
	void DrawBackGround(HDC hdc);
	void SetWorkSpaceArea(int x, int y, HDC hdc);
	void ChangeShowArea(short MouseWhell);
	void InvalidateArea(HWND hwnd);
	void SetBackGroundBM(HDC hdc);
	void Exit();

private:
	RECT WorkRect;
	RECT ShowRect;
	BOOL MouseWhellFlag;
	HBITMAP BMBackGround;
	short SizeRate;
};

