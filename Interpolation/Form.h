#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iosfwd>
#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <Strsafe.h>
#include "BasicFunct.h";
	
#include <objidl.h>					//Drawing
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "Interpolation.h";


void InitInterface(HWND hWnd);
void SearchButtonClick(HWND hWind);
void DrawString();
VOID OnPaint(HDC hdc);
VOID GDIWrite(HDC hdc, double x, double y, wchar_t* str, int FontSize);
wchar_t* StrIncert(wchar_t* str, int pos, wchar_t sym, int &len);
wchar_t* ConvertPolinom(wchar_t* polin);
double atof(char s[]);
void ErrorExit(LPCTSTR lpszFunction);
VOID GraphicDraw(HDC hdc, int n, int number);
LPTSTR DoublecutZeros(double val);
VOID DrawAxisVals(HDC hdc);
VOID DrawButtonClick(HWND hWind);
VOID DecBtnClick(HWND hWind);
VOID IncBtnClick(HWND hWind);

double stepDim = 1;
int n;
int number;

#define SearchBX 230
#define SearchBY 25
#define SearchBW 70
#define SearchBH 25
#define SEARCH_BTN_ID (100)
const wchar_t SearchBText[] = L"Calculate";

#define DrawButtonX 310
#define DrawButtonY 25
#define DrawButtonW 70
#define DrawButtonH 25
#define DRAW_BTN_ID (104)
const wchar_t DrawBtnText[] = L"Graph";

#define IncDimX 440
#define IncDimY 15
#define IncDimW 25
#define IncDimH 25
#define INCDIM_BTN_ID (105)
const wchar_t IncDimBtnText[] = L"+";

#define DecDimX 440
#define DecDimY 40
#define DecDimW 25
#define DecDimH 25
#define DECDIM_BTN_ID (106)
const wchar_t DecDimBtnText[] = L"-";

#define EditBX 20
#define EditBY 25
#define EditBW 200
#define EditBH 25
#define EDIT_ID (101)
#define EditBSize 24

#define PolinomX 350 
#define PolinomY 25
#define PolinimLenInChar 40
#define PolinomFontSize 20

#define DisplayBX 20
#define DisplayBY 75
#define DisplayBW 400
#define DisplayBH 350
#define DISPLAY_ID (103)
#define DisplayBSize 24

#define GraphicX 440
#define GraphicY 75
#define FILE_NAME "graphic.jpg"
#define OxYx	20 
#define OxYy	170
#define OYx 20
#define OYy 20
#define OXx 450
#define OXy 330
#define Ostep 20
#define OyStepCount 14
#define OxStepCount 20