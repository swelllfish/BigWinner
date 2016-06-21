#pragma once

#include "stdafx.h"


#define	WHELL_ZOOM_IN		(1)
#define	WHELL_ZOOM_OUT		(-1)
#define	WHELL_ZOOM_NON		(0)

#define	INTER_LEN_CHANGE	(1)

#define ID_BUTTON1			(1)
typedef struct _ANALYSIS_BUTTON
{
	HWND Button1;
}ANALYSIS_BUTTON;

class Analisis
{
public:
	Analisis(void);
	~Analisis(void);

	void CreateWindowButton(HWND hwnd, HINSTANCE hInstance);
	void MoveWindowButton();
	void DrawButton(LPDRAWITEMSTRUCT pdis);
	void GetFilePoint(OpFile *opfile);
	void ShowTable(HDC hdc);
	void DrawBackGround(HDC hdc);
	void SetWorkSpaceArea(int x, int y);
	void ChangeShowArea(short MouseWhell);
	void MouseAction(HWND hwnd, short x, short y, unsigned short act_type);
	void DrawCoordinate(HDC hdc);
	void InvalidateArea(HWND hwnd);
	void Exit();

private:
	PaintFun paintfun;
	OpFile *p_opfile;
	RECT  windowAreaRect;		//the Window size
	RECT  tableAreaRect;		//Table size
	int   nowInterLen;		//Changing Horrizon coordinate inter length
	int   newInterLen;	//Horrizon coordinate inter length
	int   preInterLen;  //Old Horrizon coordinate inter length
	bool  lButtonDownFlag;
	bool  tableSlideFlag;
	double tableSlideMove_x;  //Table Slide move
	char  zoomWhellChange;
	POINT preMouseLocation;
	POINT nowMouseLocation;
	POINT whellMouseLocation;
	int   mouseMove_x;	//Table move follow the cursor
	int   tableStartPoint_x;	//First Point Location
	ANALYSIS_BUTTON Analysis_Button;
};

