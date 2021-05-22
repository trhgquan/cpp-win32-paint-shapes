// Paint.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Paint.h"

#pragma comment(lib, "Comctl32.lib")

#define MAX_LOADSTRING 100

#define IMAGE_WIDTH    32
#define IMAGE_HEIGHT   32
#define BUTTON_WIDTH   32
#define BUTTON_HEIGHT  32

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Cursor pointer
HCURSOR wCursor;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//---------------------------------------------------------------

/// <summary>
/// Default pen style
/// </summary>
int defaultPenStyle = PS_DASHDOT;

/// <summary>
/// Default pen width.
/// </summary>
int defaultPenWidth = 2;

/// <summary>
/// Default pen colour.
/// </summary>
COLORREF defaultPenColour = RGB(0, 0, 0);

/// <summary>
/// Default (shape) background color
/// </summary>
COLORREF defaultShapeBackgroundColour = RGB(255, 255, 255);

/// <summary>
/// Top-left position.
/// </summary>
Point topLeft;

/// <summary>
/// Right-bottom position.
/// </summary>
Point rightBottom;

/// <summary>
/// Save the first position
/// </summary>
Point firstPosition, secondPosition;

/// <summary>
/// Preview mode.
/// </summary>
bool isDrawing = false;

/// <summary>
/// Special shapes (square, circle)
/// </summary>
bool isSpecialShape = false;

/// <summary>
/// Default shapes
/// </summary>
int shapeType = 0;        // 0 : Line
                          // 1 : Rectangle
                          // 2 : Square
                          // 3 : Ellipse
                          // 4 : Circle

/// <summary>
/// Vector of shapes drawed.
/// </summary>
std::vector<std::shared_ptr<IShape>> shapesVector;

/// <summary>
/// ShapeFactory - generating new shapes.
/// </summary>
ShapeFactory* shapeFactory = ShapeFactory::getInstance();

//---------------------------------------------------------------

/// <summary>
/// Controller - handle special actions
/// </summary>
namespace Controller {
  /// <summary>
  /// Handle shape-changing (change to another shape).
  /// </summary>
  /// <param name="id"></param>
  void HandleShapeChanging(int id) {
    switch (id) {
    case ID_DRAW_LINE: {
      shapeType = 0;
      isSpecialShape = false;
      break;
    }
    case ID_DRAW_RECTANGLE: {
      shapeType = 1;
      isSpecialShape = false;
      break;
    }
    case ID_DRAW_SQUARE: {
      shapeType = 2;
      isSpecialShape = true;
      break;
    }
    case ID_DRAW_ELLIPSE: {
      shapeType = 3;
      isSpecialShape = false;
      break;
    }
    case ID_DRAW_CIRCLE: {
      shapeType = 4;
      isSpecialShape = true;
      break;
    }
    default:
      MessageBox(NULL, L"Building in progress..", L"Info", 64);
    }
  }
}

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
      { STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    };
    TBBUTTON userButtons[] = {
      { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
      { 0, ID_DRAW_CIRCLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 1, ID_DRAW_ELLIPSE , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 2, ID_DRAW_SQUARE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 3, ID_DRAW_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 4, ID_DRAW_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0}, 
      { 5, ID_DRAW_TEXT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 6, ID_SHAPE_SELECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0}
    };
    
    // Add bitmap file.
    TBADDBITMAP tbBitmap = {
      hInst, IDB_BITMAP1
    };

    // Create a toolbar
    HWND hToolBarWnd = CreateToolbarEx(hwnd,
      WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
      ID_TOOLBAR, sizeof(tbButtons) / sizeof(TBBUTTON), HINST_COMMCTRL,
      0, tbButtons, sizeof(tbButtons) / sizeof(TBBUTTON),
      BUTTON_WIDTH, BUTTON_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT,
      sizeof(TBBUTTON)
    );

    int idx = (int)SendMessage(hToolBarWnd, TB_ADDBITMAP,
      (WPARAM)sizeof(tbBitmap) / sizeof(TBADDBITMAP),
      (LPARAM)(LPTBADDBITMAP)&tbBitmap);

    // Set image for button.
    userButtons[1].iBitmap += idx;
    userButtons[2].iBitmap += idx;
    userButtons[3].iBitmap += idx;
    userButtons[4].iBitmap += idx;
    userButtons[5].iBitmap += idx;
    userButtons[6].iBitmap += idx;
    userButtons[7].iBitmap += idx;

    SendMessage(hToolBarWnd, TB_ADDBUTTONS,
      (WPARAM)sizeof(userButtons) / sizeof(TBBUTTON),
      (LPARAM)(LPTBBUTTON)&userButtons);

    // Load cursor
    wCursor = LoadCursorFromFileW(CURSOR_BASE);
    
    return true;
  }

  /// <summary>
  /// Handle destroy event.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns></returns>
  void OnDestroy(HWND hwnd) {
    // Destroy ShapeFactory
    shapeFactory->deleteInstance();

    // Post quit message.
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
    // Destroy any drawing command.
    isDrawing = false;

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
    case ID_FILE_NEW: {
      // This just clear the screen.

      // Add confirmation box.
      MessageBox(hwnd, L"Hello", L"New", 64);

      shapesVector.clear();

      InvalidateRect(hwnd, NULL, true);
      break;
    }

    // Open button click.
    case ID_FILE_OPEN: {
      // Test file opening.
      // Clear previous shapes
      shapesVector.clear();

      MessageBox(hwnd, L"Hello", L"Open", 64);
      // Trigger fileOpenDialog here.

      std::ifstream in("shapes.txt");
      std::string buffer;

      while (std::getline(in, buffer)) {
        std::vector<std::string> tokens = Tokeniser::split(buffer, ": ");
        shapesVector.push_back(shapeFactory->getInstance()->parse(tokens[0], tokens[1]));
      }

      // Trigger redraw window.
      InvalidateRect(hwnd, NULL, true);

      break;
    }

    // Save button click.
    case ID_FILE_SAVE: {
      // Test file saving.
      std::ofstream out("shapes.txt");

      for (int i = 0; i < shapesVector.size(); ++i) {
        out << shapesVector[i]->toString() << '\n';
      }

      out.close();

      MessageBox(hwnd, L"Saved", L"Hello", 64);
      break;
    }
    
    // Handling some "special" actions
    // aka changing pen to draw special shapes.
    case ID_DRAW_RECTANGLE:
    case ID_DRAW_SQUARE:
    case ID_DRAW_ELLIPSE:
    case ID_DRAW_CIRCLE:
    case ID_DRAW_LINE:
    case ID_DRAW_TEXT:
    case ID_SHAPE_SELECT:
      Controller::HandleShapeChanging(id);
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
      shapesVector[i]->draw(hdc);
    }

    // Draw preview
    // However, I don't think this is smart
    // since this create a new pointer each time
    // updating.
    if (isDrawing) {
      std::shared_ptr<IShape> previewShape = shapeFactory->create(
        shapeType,
        topLeft,
        rightBottom,
        defaultPenStyle,
        defaultPenWidth,
        defaultPenColour,
        defaultShapeBackgroundColour
      );

      previewShape->draw(hdc);
    }

    EndPaint(hwnd, &ps);
    ReleaseDC(hwnd, hdc);
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
      secondPosition.update(x, y);

      // But with a special shape (i.e Circle or Square),
      // drawing requires 2 point standing in a diagonal.
      if (isSpecialShape) 
        Geometric::diagonalStanding(firstPosition, secondPosition);

      // Fix to correct topLeft and rightBottom
      // This only apply to shapes not line.
      Geometric::fixingPosition(
        shapeType,
        firstPosition, 
        secondPosition,
        topLeft, 
        rightBottom
      );

      // Send notify to clear the screen
      InvalidateRect(hwnd, NULL, true);
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

    firstPosition.update(x, y);
    topLeft = firstPosition;

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
    shapesVector.push_back(shapeFactory->create(
      shapeType,
      topLeft,
      rightBottom,
      defaultPenStyle,
      defaultPenWidth,
      defaultPenColour,
      defaultShapeBackgroundColour
    ));

    // Reset position

    // Clear the screen
    InvalidateRect(hwnd, NULL, true);
  }

  /// <summary>
  /// Set cursor for the application.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="hwndCursor"></param>
  /// <param name="codeHitTest"></param>
  /// <param name="msg"></param>
  /// <returns></returns>
  bool OnSetCursor(HWND hwnd, HWND hwndCursor, UINT codeHitTest, UINT msg) {
    SetCursor(wCursor);
    return true;
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
    wcex.hCursor        = LoadCursor(nullptr, CURSOR_BASE);
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
    HANDLE_MSG(hWnd, WM_SETCURSOR, EventHandler::OnSetCursor);

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
