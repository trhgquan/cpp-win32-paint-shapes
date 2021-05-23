#pragma once

#include "resource.h"

// Library
#include "Library/Tokeniser.h"
#include "Library/Shapes.h"
#include "Library/Geometric.h"

//
// Definition for some constants
// and global variables
//

// External assets
#define CURSOR_BASE L"assets/cursor/pepethefrog.cur"   // Position of the cursor.

#define MAX_LOADSTRING 100

// Image attributes
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