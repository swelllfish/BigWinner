#pragma once

#include "stdafx.h"

class Analisis
{
public:
	Analisis(void);
	~Analisis(void);

	void DrawBackGround(HDC hdc);
	void SetWorkSpaceArea(int x, int y, HDC hdc);
	void ChangeShowArea(short MouseWhell);
	void InvalidateArea(HWND hwnd);

private:
	RECT WorkRect;
	RECT ShowRect;
	RECT NewRect;
	BOOL MouseWhellFlag;
	short SizeRate;
};

