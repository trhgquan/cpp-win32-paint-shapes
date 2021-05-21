// Paint.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Paint.h"
#include "Library/Tokeniser.h"
#include "Library/Shapes.h"

#include<WindowsX.h>
#include<commctrl.h>

#pragma comment(lib, "Comctl32.lib")

#define MAX_LOADSTRING 100

#define IMAGE_WIDTH    64
#define IMAGE_HEIGHT   64
#define BUTTON_WIDTH   64
#define BUTTON_HEIGHT  64

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
/// Vector of shapes drawed.
/// </summary>
std::vector<std::shared_ptr<IShape>> shapesVector;

/// <summary>
/// Position from.
/// </summary>
int fromX, fromY;

/// <summary>
/// Position to.
/// </summary>
int toX, toY;

/// <summary>
/// Preview mode.
/// </summary>
bool isDrawing = false;

/// <summary>
/// Special shapes (square, circle)
/// </summary>
bool isSpecialShape = false;

//---------------------------------------------------------------

/// <summary>
/// Handling events.
/// </summary>
namespace EventHandler {
  /// <summary>
  /// Handle OnCreate event
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="lpCreateStruct"></param>
  /// <returns></returns>
  BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {
    // Load default system fonts.
    LOGFONT lf;
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
    HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth, lf.lfEscapement, lf.lfOrientation,
      lf.lfWeight, lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, lf.lfOutPrecision,
      lf.lfClipPrecision, lf.lfQuality, lf.lfPitchAndFamily, lf.lfFaceName);

    // Add buttons.
    InitCommonControls();
    TBBUTTON tbButtons[] = {
      { STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
      { STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
      {STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0}
    };

    // Create a toolbar
    HWND hToolBarWnd = CreateToolbarEx(hwnd,
      WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
      ID_TOOLBAR, sizeof(tbButtons) / sizeof(TBBUTTON), HINST_COMMCTRL,
      0, tbButtons, sizeof(tbButtons) / sizeof(TBBUTTON),
      BUTTON_WIDTH, BUTTON_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT,
      sizeof(TBBUTTON)
    );

    return true;
  }

  /// <summary>
  /// Handle destroy event.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns></returns>
  void OnDestroy(HWND hwnd) {
    PostQuitMessage(0);
  }

  /// <summary>
  /// Handle OnCommand Event.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="id"></param>
  /// <param name="hwndCtl"></param>
  /// <param name="codeNotify"></param>
  /// <returns></returns>
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

    // New button click.
    case ID_FILE_NEW:
      // Add confirmation box.
      MessageBox(hwnd, L"Hello", L"New", 64);


      break;

    // Open button click.
    case ID_FILE_OPEN:
      MessageBox(hwnd, L"Hello", L"Open", 64);

      // Trigger fileOpenDialog here.

      break;

    // Save button click.
    case ID_FILE_SAVE:
      MessageBox(hwnd, L"Hello", L"Save", 64);
      break;
    }
 }

  /// <summary>
  /// Handle OnPaint event.
  /// </summary>
  /// <param name="hwnd"></param>
  void OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    
    // Draw list of shapes.
    for (int i = 0; i < shapesVector.size(); ++i) {
      // Draw a line
      if (shapesVector[i]->type() == "line") {
        std::shared_ptr<LineShape> line = std::static_pointer_cast<LineShape>(shapesVector[i]);
        line->draw(hdc);
      }

      // Draw a rectangle.
      if (shapesVector[i]->type() == "rectangle") {
        std::shared_ptr<RectangleShape> rectangle = std::static_pointer_cast<RectangleShape>(shapesVector[i]);
        rectangle->draw(hdc);
      }

      // Draw a square.
      if (shapesVector[i]->type() == "square") {
        std::shared_ptr<SquareShape> square = std::static_pointer_cast<SquareShape>(shapesVector[i]);
        square->draw(hdc);
      }

      // Draw an ellipse.
      if (shapesVector[i]->type() == "ellipse") {
        std::shared_ptr<EllipseShape> ellipse = std::static_pointer_cast<EllipseShape>(shapesVector[i]);
        ellipse->draw(hdc);
      }

      // Draw a circle.
      if (shapesVector[i]->type() == "circle") {
        std::shared_ptr<CircleShape> circle = std::static_pointer_cast<CircleShape>(shapesVector[i]);
        circle->draw(hdc);
      }
    }

    // Draw preview
    HPEN hPen = CreatePen(PS_DASHDOT, 3, RGB(255, 0, 0));
    SelectObject(hdc, hPen);
    MoveToEx(hdc, fromX, fromY, NULL);
    Rectangle(hdc, fromX, fromY, toX, toY);

    EndPaint(hwnd, &ps);
  }

  /// <summary>
  /// Handle mouse move.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="x"></param>
  /// <param name="y"></param>
  /// <param name="keyFlags"></param>
  void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
    // Do nothing.
    if (isDrawing) {
      // With a normal shape, you can draw wherever you like.
      if (!isSpecialShape) {
        toX = x;
        toY = y;
      }

      // But with a special shape (i.e Circle or Square),
      // drawing requires 2 point standing in a diagonal.
      else {
        int dx = x - fromX;
        int dy = y - fromY;

        toX = fromX + max(dx, dy);
        toY = fromY + max(dx, dy);
      }
      
      // Send notify to clear the screen
      InvalidateRect(hwnd, NULL, true);
    }

    else {
      // Reset all variables.
      fromX = fromY = toX = toY = 0;
    }
  }

  /// <summary>
  /// Handle left button hold.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="fDoubleClick"></param>
  /// <param name="x"></param>
  /// <param name="y"></param>
  /// <param name="keyFlags"></param>
  void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {
    isDrawing = true;

    fromX = x;
    fromY = y;

    HDC hdc = GetDC(hwnd);

    MoveToEx(hdc, x, y, NULL);
  }

  /// <summary>
  /// Handle left button release.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="x"></param>
  /// <param name="y"></param>
  /// <param name="keyFlags"></param>
  void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
    // Release drawing status.
    isDrawing = false;

    // Add shape to shapes vector.
    shapesVector.push_back(std::make_shared<RectangleShape>(
      Point(fromX, fromY),
      Point(toX, toY),
      PS_DASHDOT,
      3,
      RGB(255, 0, 0)
    ));

    InvalidateRect(hwnd, NULL, true);
  }
}
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
    HANDLE_MSG(hWnd, WM_CREATE, EventHandler::OnCreate);
    HANDLE_MSG(hWnd, WM_COMMAND, EventHandler::OnCommand);
    HANDLE_MSG(hWnd, WM_PAINT, EventHandler::OnPaint);
    HANDLE_MSG(hWnd, WM_DESTROY, EventHandler::OnDestroy);
    HANDLE_MSG(hWnd, WM_LBUTTONDOWN, EventHandler::OnLButtonDown);
    HANDLE_MSG(hWnd, WM_LBUTTONUP, EventHandler::OnLButtonUp);
    HANDLE_MSG(hWnd, WM_MOUSEMOVE, EventHandler::OnMouseMove);

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
