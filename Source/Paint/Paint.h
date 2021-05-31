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

// Image attributes
#define IMAGE_WIDTH    32
#define IMAGE_HEIGHT   32
#define BUTTON_WIDTH   32
#define BUTTON_HEIGHT  32

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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

/// <summary>
/// Top-left and RightBottom position.
/// </summary>
Point topLeft, rightBottom;

/// <summary>
/// Save the first position
/// </summary>
Point firstPosition, secondPosition;

/// <summary>
/// Default Shape graphic.
/// </summary>
ShapeGraphic defaultShapeGraphic(
  PS_SOLID,
  1,
  RGB(0, 0, 0),
  RGB(255, 255, 255)
);

/// <summary>
/// This variable tolds that this art
/// has changed or not.
/// </summary>
bool hasChanged = false;

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