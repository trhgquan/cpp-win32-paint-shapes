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
    // Get client rectangle.
    GetClientRect(hwnd, &hClientRect);

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
    hToolbarWnd = CreateToolbarEx(hwnd,
      WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
      ID_TOOLBAR, sizeof(tbButtons) / sizeof(TBBUTTON), HINST_COMMCTRL,
      0, tbButtons, sizeof(tbButtons) / sizeof(TBBUTTON),
      BUTTON_WIDTH, BUTTON_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT,
      sizeof(TBBUTTON)
    );

    int idx = (int)SendMessage(hToolbarWnd, TB_ADDBITMAP,
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

    SendMessage(hToolbarWnd, TB_ADDBUTTONS,
      (WPARAM)sizeof(userButtons) / sizeof(TBBUTTON),
      (LPARAM)(LPTBBUTTON)&userButtons);

    // Check default penstyle item.
    PenstyleController::checkMenuItem(
      hwnd,
      PenstyleController::getMenuItemFromPen(
        hwnd
      )
    );

    // Create a status bar
    hStatusBarWnd = CreateStatusWindow(
      WS_CHILD | WS_VISIBLE,
      L"",
      hwnd,
      ID_STATUSBAR
    );

    // Divide the statusbar into parts
    int statusbarParts[] = {
      300,
      600,
      hClientRect.right - hClientRect.left
    };

    SendMessage(
      hStatusBarWnd,
      SB_SETPARTS,
      3,
      (LPARAM)&statusbarParts
    );

    // Set text for each parts.
    SendMessage(
      hStatusBarWnd,
      SB_SETTEXT,
      (WPARAM)0,
      (LPARAM)L"Hát để bắt đầu (đùa á, thử vẽ gì đi)."
    );
    SendMessage(
      hStatusBarWnd,
      SB_SETTEXT,
      (WPARAM)1,
      (LPARAM)L"Vị trí con trỏ: (0, 0)"
    );
    SendMessage(
      hStatusBarWnd,
      SB_SETTEXT,
      (WPARAM)2,
      (LPARAM)L"Trang vẽ mới."
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
    case ID_SHAPE_DELETE:
    case ID_SHAPE_COPY:
    case ID_SHAPE_CUT:
    case ID_SHAPE_PASTE:
      ShapeController::handleShapeActions(hwnd, id);
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

    // Draw temporary review shape when drawing a new shape.
    if (programStatus & IS_DRAWING) {
      shapeFactory->create(
        shapeType,
        topLeft,
        rightBottom,
        defaultShapeGraphic
      )->draw(hdcCompatible);
    }

    // Draw current selection shape.
    if (programStatus & IS_SELECTING) {
      selectionShape->setTopLeft(topLeft);
      selectionShape->setRightBottom(rightBottom);
      selectionShape->draw(hdcCompatible);
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
    if (programStatus & IS_STARTED) {
      // Moving when is on drawing mode.
      if (programStatus & IS_DRAWING) {
        // With a normal shape, you can draw wherever you like.
        secondPosition.update(x, y);

        // But with a special shape (i.e Circle or Square),
        // drawing requires 2 point standing in a diagonal.
        if (programStatus & IS_SPECIAL) {
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
      if (programStatus & IS_MOVING) {
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

      // Moving when is on selection mode.
      if (programStatus & IS_SELECTING) {
        secondPosition.update(x, y);

        Geometric::fixingPosition(
          RECTANGLE_SHAPE,
          firstPosition,
          secondPosition,
          topLeft,
          rightBottom
        );
      }

      // Send notify to clear the screen.
      InvalidateRect(hwnd, NULL, false);
    }

    StatusbarController::onMouseMove(hStatusBarWnd, x, y);
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
    programStatus |= IS_STARTED;
    
    // Update first position
    firstPosition.update(x, y);
    
    if (programStatus & IS_DRAWING) {
      topLeft = firstPosition;
    }

    if (programStatus & IS_SELECTING) {
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
    if (programStatus & IS_STARTED) {
      // Release started status.
      programStatus &= ~IS_STARTED;

      // Officialy this workspace has been painted.
      programStatus |= IS_CHANGED;

      // Things to do after draw
      if (programStatus & IS_DRAWING) {
        std::shared_ptr<IShape> newShape = shapeFactory->create(
          shapeType,
          topLeft,
          rightBottom,
          defaultShapeGraphic
        );

        // Add shape to shapes vector.
        shapesVector.push_back(newShape);

        // Write to statusbar.
        StatusbarController::onCreateShape(hStatusBarWnd, newShape);
      }

      // Things to do after select.
      if (programStatus & IS_SELECTING) {
        // Flags marked that is there any shapes
        // fits inside selection zone or not.
        bool hasSelected = false;

        // Index of selected shape.
        int i = (int)shapesVector.size() - 1;

        // Iterates through the vector to find
        // who's the millionare.
        for (; i >= 0; --i) {
          if (shapesVector[i]->in(
            selectionShape->topLeft(),
            selectionShape->rightBottom()
          )) {
            // Found, then break.
            selectedShape = shapesVector[i];
            hasSelected = true;
            break;
          }
        }

        // Push the selected shape to the back,
        // then remove it from the original position.
        if (hasSelected) {
          shapesVector.erase(shapesVector.begin() + i);
          shapesVector.push_back(selectedShape);

          // Set statusbar text.
          StatusbarController::onSelectShape(hStatusBarWnd, selectedShape);
        }
      }

      // Things to do after move.
      if (programStatus & IS_MOVING) {
        StatusbarController::onMoveShape(hStatusBarWnd, selectedShape);
      }

      // Redraw window.
      InvalidateRect(hwnd, NULL, false);
    }
  }

  /// <summary>
  /// Handle window resize.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="state"></param>
  /// <param name="cx"></param>
  /// <param name="cy"></param>
  void OnSize(HWND hwnd, UINT state, int cx, int cy) {
    // Demand resizing toolbar.
    SendMessage(hToolbarWnd, WM_SIZE, 0, 0);

    // Demand resizing status bar.
    SendMessage(hStatusBarWnd, WM_SIZE, 0, 0);
  }
}