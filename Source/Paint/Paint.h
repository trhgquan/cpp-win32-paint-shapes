#pragma once

#include "resource.h"

// Library
#include "Library/Tokeniser.h"
#include "Library/ShapeGraphic.h"
#include "Library/Shapes.h"
#include "Library/Geometric.h"
#include "Library/Bitmap.h"

//
// Definition for some constants
// and global variables
//

#define MAX_LOADSTRING 100

//
// Image attributes
//
//
#define IMAGE_WIDTH    32
#define IMAGE_HEIGHT   32
#define BUTTON_WIDTH   32
#define BUTTON_HEIGHT  32

//
// Shapes ID
//
//
#define LINE_SHAPE 0
#define RECTANGLE_SHAPE 1
#define SQUARE_SHAPE 2
#define ELLIPSE_SHAPE 3
#define CIRCLE_SHAPE 4

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//
// These variables are handles of GUI.
//
//
HWND hToolbarWnd;                // the toolbar handle.
HWND hStatusBarWnd;              // the status bar handle.

//
// These variables holding handles for dialogs.
//
//

// Default structure variable for 
// - the open file dialog,
// - the save file dialog.
// - the export to bitmap dialog.
OPENFILENAME hOpenFile, 
             hSaveFile,
             hExportFile;

// Default structure variable for
// - the choose colour dialog.
CHOOSECOLOR hChooseColour;

//
// These variables are used during graphic procedures
// e.g WM_PAINT (to prevent flicks)
//

/// <summary>
/// Client area rectangle.
/// </summary>
RECT hClientRect;

/// <summary>
/// HDC of the screen.
/// </summary>
HDC hdcScreen;

/// <summary>
/// Compatible HDC of the screen.
/// </summary>
HDC hdcCompatible;

/// <summary>
/// BeginPaint zone.
/// </summary>
HDC hdcPaint;

/// <summary>
/// Handle of the screen's bitmap
/// </summary>
HBITMAP hBitmap;

/// <summary>
/// Handle to an old object
/// </summary>
HANDLE hOldObject;

//
// These variables are used during drawing.
//
//

/// <summary>
/// Top-left and RightBottom position.
/// </summary>
Point topLeft, rightBottom;

/// <summary>
/// Save the first and second position
/// </summary>
Point firstPosition, secondPosition;

/// <summary>
/// Default Shape graphic.
/// </summary>
ShapeGraphic defaultShapeGraphic(
  PS_SOLID,           // Line (border) style
  1,                  // Line (border) width
  RGB(0, 0, 0),       // Line (border) colour

  DC_BRUSH,           // Background brush.
                      // NULL_BRUSH is transparent.
  RGB(255, 255, 255)  // Background colour.
);

/// <summary>
/// All flags of the program.
/// </summary>
const enum {
  IS_CHANGED = 1 << 0,
  IS_STARTED = 1 << 1,
  IS_DRAWING = 1 << 2,
  IS_MOVING = 1 << 3,
  IS_SELECTING = 1 << 4,
  IS_SPECIAL = 1 << 5
};

// Default program status.
int programStatus = IS_DRAWING;

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

//
// These variables are used during moving/selection
//
//

/// <summary>
/// Current selected shape.
/// </summary>
std::shared_ptr<IShape> selectedShape;

/// <summary>
/// Selection shape graphic.
/// </summary>
ShapeGraphic selectionShapeGraphic(
  PS_DOT,
  1,
  RGB(0, 0, 0),

  NULL_BRUSH,
  RGB(255, 255, 255)
);

/// <summary>
/// Selection rectangle.
/// </summary>
std::shared_ptr<RectangleShape> selectionShape(
  new RectangleShape(
    { 0, 0 },
    { 0, 0 },
    selectionShapeGraphic
  )
);
