﻿// Paint.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Paint.h"
#include<WindowsX.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//---------------------------------------------------------------

/// <summary>
/// Handle OnCreate event
/// </summary>
/// <param name="hwnd"></param>
/// <param name="lpCreateStruct"></param>
/// <returns></returns>
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

/// <summary>
/// Handle destroy event.
/// </summary>
/// <param name="hwnd"></param>
/// <returns></returns>
void OnDestroy(HWND hwnd);

/// <summary>
/// Handle OnCommand Event.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="id"></param>
/// <param name="hwndCtl"></param>
/// <param name="codeNotify"></param>
/// <returns></returns>
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

/// <summary>
/// Handle OnPaint event.
/// </summary>
/// <param name="hwnd"></param>
void OnPaint(HWND hwnd);

/// <summary>
/// Handle left button release.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="keyFlags"></param>
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);

/// <summary>
/// Handle left button hold.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="fDoubleClick"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="keyFlags"></param>
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

/// <summary>
/// Handle mouse move.
/// </summary>
/// <param name="hwnd"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="keyFlags"></param>
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
//---------------------------------------------------------------

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
    HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
    HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
    HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
    HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//---------------------------------------------------------------

// Handle OnCreate Event.
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {

  return true;
}

// Handle OnDestroy Event.
void OnDestroy(HWND hwnd) {
  PostQuitMessage(0);
}

// Handle OnCommand Event.
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
  switch (id) {
  // About click.
  case IDM_ABOUT:
    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
    break;

    // Exit click.
  case IDM_EXIT:
    DestroyWindow(hwnd);
    break;
  }
}

// Handle OnPaint Event.
void OnPaint(HWND hwnd) {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  // TODO: Add any drawing code that uses hdc here...
  
  EndPaint(hwnd, &ps);
}

// Handle mouse move.
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
  // Do nothing.
}

// Handle mouse left-click.
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
  // Do nothing.
  HDC hdc = GetDC(hwnd);
  SetPixel(hdc, x, y, RGB(255, 0, 0));
  ReleaseDC(hwnd, hdc);
}

// Handle mouse left-click release.
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
  // Do nothing.
}

//---------------------------------------------------------------

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}