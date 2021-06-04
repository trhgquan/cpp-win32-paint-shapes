#pragma once

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
      { STD_DELETE, ID_SHAPE_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { STD_CUT, ID_SHAPE_CUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { STD_COPY, ID_SHAPE_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { STD_PASTE, ID_SHAPE_PASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
      { 0, ID_DRAW_CIRCLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 1, ID_DRAW_ELLIPSE , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 2, ID_DRAW_SQUARE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 3, ID_DRAW_RECTANGLE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 4, ID_DRAW_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 5, ID_SHAPE_SELECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
      { 6, ID_SHAPE_MOVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},
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
    userButtons[6].iBitmap += idx;
    userButtons[7].iBitmap += idx;
    userButtons[8].iBitmap += idx;
    userButtons[9].iBitmap += idx;
    userButtons[10].iBitmap += idx;
    userButtons[11].iBitmap += idx;
    userButtons[12].iBitmap += idx;

    SendMessage(hToolBarWnd, TB_ADDBUTTONS,
      (WPARAM)sizeof(userButtons) / sizeof(TBBUTTON),
      (LPARAM)(LPTBBUTTON)&userButtons);

    // Check default penstyle item.
    PenstyleController::checkMenuItem(
      hwnd,
      PenstyleController::getMenuItemFromPen(
        hwnd
      )
    );

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
    switch (id) {
      // About click.
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
      break;

      // Exit click.
    case IDM_EXIT:
      DestroyWindow(hwnd);
      break;

    // Point file actions to file actions controller
    case ID_FILE_NEW:
    case ID_FILE_OPEN:
    case ID_FILE_SAVE:
    case ID_FILE_EXPORT:
      FileController::handleFileActions(hwnd, id);
      break;

    // Point draw action to shape controller
    case ID_DRAW_RECTANGLE:
    case ID_DRAW_SQUARE:
    case ID_DRAW_ELLIPSE:
    case ID_DRAW_CIRCLE:
    case ID_DRAW_LINE:
    case ID_SHAPE_SELECT:
    case ID_SHAPE_MOVE:
      ShapeController::handleShapeActions(id);
      break;
    
    // Colour action to colour controller.
    case ID_COLOUR_BACKGROUND:
    case ID_COLOUR_LINE:
      ColourController::handleColourActions(hwnd, id);
      break;

    // Pen change action to pen controller.
    case ID_PENSTYLE_SOLID:
    case ID_PENSTYLE_DOT:
    case ID_PENSTYLE_DASH:
    case ID_PENSTYLE_DASH_DOT:
    case ID_PENSTYLE_DASH_DOT_DOT:
      PenstyleController::handlePenstyleActions(hwnd, id);
      break;
    }
  }

  /// <summary>
  /// Handle OnPaint event.
  /// </summary>
  /// <param name="hwnd"></param>
  void OnPaint(HWND hwnd) {
    // Get client area.
    GetClientRect(hwnd, &hClientRect);

    hdcScreen = GetDC(hwnd);

    PAINTSTRUCT ps;
    
    // Create paint graphic area.
    hdcPaint = BeginPaint(hwnd, &ps);
    hdcCompatible = CreateCompatibleDC(hdcPaint);
    hBitmap = CreateCompatibleBitmap(
      hdcScreen,
      hClientRect.right - hClientRect.left,
      hClientRect.bottom - hClientRect.top
    );

    hOldObject = SelectObject(hdcCompatible, hBitmap);

    // Fill that area with the background.
    FillRect(
      hdcCompatible,
      &hClientRect,
      (HBRUSH)(COLOR_BTNFACE + 1)
    );

    // Select the null brush.
    SelectObject(hdcCompatible, GetStockObject(NULL_BRUSH));

    // Draw list of shapes.
    for (int i = 0; i < shapesVector.size(); ++i) {
      shapesVector[i]->draw(hdcCompatible);
    }

    // Draw preview
    // However, I don't think this is smart
    // since this create a new pointer each time
    // updating.
    if (isPreviewing) {
      shapeFactory->create(
        shapeType,
        topLeft,
        rightBottom,
        defaultShapeGraphic
      )->draw(hdcCompatible);
    }

    // Copy bits from the buffer to the screen.
    BitBlt(
      hdcPaint,
      0, 0,
      hClientRect.right - hClientRect.left,
      hClientRect.bottom - hClientRect.top,
      hdcCompatible,
      0, 0,
      SRCCOPY
    );

    // Release handles.
    SelectObject(hdcCompatible, hOldObject);
    DeleteObject(hBitmap);
    DeleteDC(hdcCompatible);

    EndPaint(hwnd, &ps);
    ReleaseDC(hwnd, hdcScreen);
    ReleaseDC(hwnd, hdcPaint);
  }

  /// <summary>
  /// Handle mouse move.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="x"></param>
  /// <param name="y"></param>
  /// <param name="keyFlags"></param>
  void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
    if (started) {
      // Moving when is on drawing mode.
      if (isDrawing) {
        // With a normal shape, you can draw wherever you like.
        secondPosition.update(x, y);

        // But with a special shape (i.e Circle or Square),
        // drawing requires 2 point standing in a diagonal.
        if (isSpecialShape) {
          Geometric::diagonalStanding(firstPosition, secondPosition);
        }

        // Fix to correct topLeft and rightBottom
        // This only apply to shapes not line.
        Geometric::fixingPosition(
          shapeType,
          firstPosition,
          secondPosition,
          topLeft,
          rightBottom
        );
      }

      // Moving when is on moving mode.
      if (isMoving) {
        // Update the second position, so you can create a vector.
        secondPosition.update(x, y);

        // Calculate the vector.
        int dx = secondPosition.x() - firstPosition.x();
        int dy = secondPosition.y() - firstPosition.y();

        // Move the shape.
        selectedShape->move(dx, dy);

        // Update current position
        firstPosition = secondPosition;
      }

      // Send notify to clear the screen.
      InvalidateRect(hwnd, NULL, false);
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
    // Tell that the user started.
    started = true;
    
    // Update first position
    firstPosition.update(x, y);
    
    if (isDrawing) {
      topLeft = firstPosition;
    }
    
    // Get device context to move.
    HDC hdc = GetDC(hwnd);
    
    // Move to the (x, y)
    MoveToEx(hdc, x, y, NULL);

    ReleaseDC(hwnd, hdc);
  }

  /// <summary>
  /// Handle left button release.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="x"></param>
  /// <param name="y"></param>
  /// <param name="keyFlags"></param>
  void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
    if (started) {
      // Release started status.
      started = false;

      // Officialy this workspace has been painted.
      hasChanged = true;

      if (isDrawing) {
        // Add shape to shapes vector.
        shapesVector.push_back(shapeFactory->create(
          shapeType,
          topLeft,
          rightBottom,
          defaultShapeGraphic
        ));
      }
    }
  }
}