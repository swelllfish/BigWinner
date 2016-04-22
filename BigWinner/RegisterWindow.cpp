#include "stdafx.h"

/**Bigwinner register window main function**/

#define ID_TIMER 1
#define TIMER_CLK 13

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
WNDCLASS SetWndClass(HINSTANCE, TCHAR []);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("BigWinner");
	HWND hwnd;
	MSG msg;
	WNDCLASS Mwndclass;

	Mwndclass = SetWndClass(hInstance, szAppName);

	if(!RegisterClass(&Mwndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	RECT hRect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&hRect, 0);	//��ȡ��������С

	hwnd = CreateWindow(szAppName, TEXT("Big-Winner"), 
		WS_OVERLAPPEDWINDOW,
		hRect.left, 
		hRect.top, 
		hRect.right, 
		hRect.bottom, 
		NULL, 
		NULL,
		hInstance, 
		NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

WNDCLASS SetWndClass(HINSTANCE hInstance, TCHAR szAppName[])
{
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(146, 168, 209));
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = (LPCWSTR)szAppName;

	return wndclass;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC *hdc;
	static OpFile opfile;
	static Analisis analisis;
	PAINTSTRUCT ps;

	switch(message)
	{
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER, TIMER_CLK, NULL);
		opfile.FileAnalise();
		analisis.GetFilePoint(&opfile);
		return 0;

	case WM_SIZE:
		hdc = new HDC;
		*hdc = GetDC(hwnd);
		analisis.SetWorkSpaceArea(LOWORD(lParam), HIWORD(lParam));
		ReleaseDC(hwnd, *hdc);

		delete(hdc);
		return 0;
		
	case WM_PAINT:
		hdc = new HDC;
		*hdc = BeginPaint(hwnd, &ps);
		analisis.ShowTable(hdc, &opfile);
		EndPaint(hwnd, &ps);

		delete(hdc);
		return 0;

	case WM_MOUSEWHEEL:
		analisis.ChangeShowArea(HIWORD(wParam));
		return 0;

	case WM_LBUTTONDOWN:
		analisis.MouseAction(hwnd, LOWORD(lParam), HIWORD(lParam), WM_LBUTTONDOWN);
		return 0;

	case WM_LBUTTONUP:
		analisis.MouseAction(hwnd, LOWORD(lParam), HIWORD(lParam), WM_LBUTTONUP);
		return 0;

	case WM_MOUSEMOVE:
		analisis.MouseAction(hwnd, LOWORD(lParam), HIWORD(lParam), WM_MOUSEMOVE);
		return 0;

	case WM_TIMER:
		analisis.InvalidateArea(hwnd);
		return 0;

	case WM_DESTROY:
		opfile.Exit();
		analisis.Exit();
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}