#pragma once

/// <summary>
/// ShapeController - handle special actions
/// with Shape
/// </summary>
namespace ShapeController {
  /// <summary>
  /// Reset shape drawing.
  /// aka reset all shapes.
  /// </summary>
  /// <param name="hwnd"></param>
  void resetShapeDrawing(HWND hwnd) {
    // Reset untouch status.
    programStatus &= ~IS_CHANGED;

    // Prevent drawing last shape and last selection.
    programStatus &= ~IS_DRAWING;
    programStatus &= ~IS_SELECTING;

    // Reset all shapes.
    shapesVector.clear();
  }

  /// <summary>
  /// Remove a shape from the vector
  /// and the screen also.
  /// </summary>
  /// <param name="hwnd"></param>
  void removeShapeDrawing(HWND hwnd) {
    // Remove current selected shape.
    shapesVector.pop_back();

    // Notify to redraw the screen.
    InvalidateRect(hwnd, NULL, false);
  }

  /// <summary>
  /// Copy a shape,
  /// basically this just re-point the selected
  /// to the back of the vector (selected one).
  /// </summary>
  /// <param name="hwnd"></param>
  void copyShapeDrawing(HWND hwnd) {
    if (shapesVector.size() > 0) {
      selectedShape = shapesVector.back();
    }

    else {
      throw std::exception("No shapes found!");
    }
  }

  /// <summary>
  /// Cut a shape,
  /// basically this point the selected to the last
  /// and remove it from the vector.
  /// </summary>
  /// <param name="hwnd"></param>
  void cutShapeDrawing(HWND hwnd) {
    try {
      copyShapeDrawing(hwnd);

      removeShapeDrawing(hwnd);
    }

    catch (const std::exception& e) {
      throw e;
    }
  }

  /// <summary>
  /// Paste a shape,
  /// aka insert currently selected shape to the back
  /// of the vector.
  /// </summary>
  /// <param name="hwnd"></param>
  void pasteShapeDrawing(HWND hwnd) {
    // Prevent pasting nulls
    if (selectedShape.use_count() > 0) {
      // Create a new shape.
      std::shared_ptr<IShape> cloneShape = selectedShape->cloneShape();

      shapesVector.push_back(cloneShape);

      // And copy the newly-created pointer.
      copyShapeDrawing(hwnd);

      // Redraw the screen.
      InvalidateRect(hwnd, NULL, false);
    }

    else {
      throw std::exception("No shapes selected!");
    }
  }

  /// <summary>
  /// Handle shape-changing (change to another shape).
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="id"></param>
  void handleShapeActions(HWND hwnd, int id) {
    // Reset all status.
    programStatus &= ~programStatus;

    switch (id) {
    case ID_DRAW_LINE: {
      shapeType = LINE_SHAPE;
      programStatus = IS_DRAWING;
      break;
    }
    case ID_DRAW_RECTANGLE: {
      shapeType = RECTANGLE_SHAPE;
      programStatus = IS_DRAWING;
      break;
    }
    case ID_DRAW_SQUARE: {
      shapeType = SQUARE_SHAPE;
      programStatus = IS_DRAWING | IS_SPECIAL;
      break;
    }
    case ID_DRAW_ELLIPSE: {
      shapeType = ELLIPSE_SHAPE;
      programStatus = IS_DRAWING;
      break;
    }
    case ID_DRAW_CIRCLE: {
      shapeType = CIRCLE_SHAPE;
      programStatus = IS_DRAWING | IS_SPECIAL;
      break;
    }
    case ID_SHAPE_MOVE: {
      if (shapesVector.size() > 0) {
        // Set Moving flag to true.
        programStatus = IS_MOVING;

        // And point current last shape to it.
        selectedShape = shapesVector.back();
      }

      else {
        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Di chuyển] Không vẽ gì sao di chuyển!"
        );
      }
      break;
    }
    case ID_SHAPE_SELECT: {
      programStatus = IS_SELECTING;
      break;
    }
    case ID_SHAPE_DELETE:
    case ID_EDITMENU_DELETE: 
    case ID_HOTKEY_DELETE: {
      try {
        if (shapesVector.size() > 0) {
          // User needs to confirming the process before going further.
          int confirmation = NotificationDialog::deleteComfirmation(hwnd);

          if (IDYES == confirmation) {
            removeShapeDrawing(hwnd);
          }

          else {
            throw std::runtime_error("Deletion canceled");
          }
        }

        else {
          throw std::exception("No shape found!");
        }

        // Update mode.
        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Xóa hình] Xóa thành công."
        );
      }

      catch (const std::runtime_error& e) {
        UNREFERENCED_PARAMETER(e);

        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Xóa] Không đồng ý ai dám xóa!"
        );
      }

      catch (const std::exception& e) {
        UNREFERENCED_PARAMETER(e);

        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Xóa] Không vẽ gì sao xóa!"
        );
      }
      
      break;
    }
    case ID_SHAPE_COPY:
    case ID_EDITMENU_COPY:
    case ID_HOTKEY_COPY: {
      try {
        ShapeController::copyShapeDrawing(hwnd);

        // Update status bar.
        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Sao chép] Chép thành công!"
        );
      }
      
      catch (const std::exception& e) {
        UNREFERENCED_PARAMETER(e);

        // Update status bar.
        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Sao chép] Không vẽ gì sao chép!"
        );
      }
      break;
    }
    case ID_SHAPE_CUT:
    case ID_EDITMENU_CUT:
    case ID_HOTKEY_CUT: {
      try {
        ShapeController::cutShapeDrawing(hwnd);

        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Cắt] Cắt thành công!"
        );
      }
      
      catch (const std::exception& e) {
        UNREFERENCED_PARAMETER(e);

        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Cắt] Không vẽ gì sao cắt!"
        );
      }

      break;
    }
    case ID_SHAPE_PASTE:
    case ID_EDITMENU_PASTE:
    case ID_HOTKEY_PASTE: {
      try {
        ShapeController::pasteShapeDrawing(hwnd);

        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Dán] Dán thành công!"
        );
      }

      catch (const std::exception& e) {
        UNREFERENCED_PARAMETER(e);

        SendMessage(
          hStatusBarWnd,
          SB_SETTEXTW,
          (WPARAM)0,
          (LPARAM)L"[Dán] Không vẽ gì sao dán!"
        );
      }
      
      break;
    }
    }
  }
}

/// <summary>
/// Controlling file actions.
/// </summary>
namespace FileController {
  /// <summary>
  /// Create a new file by clearing the screen
  /// and the shape vector.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleFileNew(HWND hwnd) {
    // Reset screen.
    ShapeController::resetShapeDrawing(hwnd);

    // Call for clear screen.
    RedrawWindow(hwnd, NULL, NULL,
      RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);

    // Set statusbar text.
    SendMessage(
      hStatusBarWnd,
      SB_SETTEXTW,
      (WPARAM)2,
      (LPARAM)L"Trang vẽ mới"
    );

    // Inform the user
    MessageBox(hwnd,
      L"Mới tạo trang vẽ mới ấy!",
      L"Ê!",
      MB_ICONINFORMATION
    );
  }

  /// <summary>
  /// Open FileOpenDialog, user choose a file,
  /// then load shapes into vectors and redraw the screen.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleFileOpen(HWND hwnd) {
    try {
      // Open file dialog and get file path.
      std::wstring filePath = FileDialog::openFileDialog(hwnd);
      std::ifstream in(filePath.c_str());
      std::string buffer;

      // Clear all shapes on screen and in vectors.
      ShapeController::resetShapeDrawing(hwnd);

      // Read shapes to a vector.
      while (std::getline(in, buffer)) {
        std::vector<std::string>tokens = Tokeniser::split(buffer, ": ");
        shapesVector.push_back(shapeFactory->parse(
          tokens.at(0),
          tokens.at(1)
        ));
      }

      // Close file open handle.
      in.close();

      // Call redraw screen.
      RedrawWindow(hwnd, NULL, NULL,
        RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW);

      // Update statusbar.
      SendMessage(
        hStatusBarWnd,
        SB_SETTEXT,
        (WPARAM)2,
        (LPARAM)filePath.c_str()
      );
      
      // Send message box informs that user has opened the file sucessfully.
      MessageBox(
        hwnd,
        L"Đã mở trang vẽ thành công!",
        L"Ê!",
        64
      );
    }
 
    // Throw exception for error while opening.
    catch (const std::underflow_error& e) {
      throw e;
    }
    
    catch (const std::exception& e) {
      // Do nothing.
      // Since the screen remains same.
      throw e;
    }
  }

  /// <summary>
  /// Open FileSaveDialog, user choose a path and a name,
  /// then write shapesVector to it.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleFileSaveAs(HWND hwnd) {
    try {
      std::wstring filePath = FileDialog::saveFileDialog(hwnd);
      std::ofstream out(filePath);

      for (int i = 0; i < shapesVector.size(); ++i) {
        out << shapesVector[i]->toString() << '\n';
      }

      out.close();

      // Set statusbar
      SendMessage(
        hStatusBarWnd,
        SB_SETTEXT,
        (WPARAM)2,
        (LPARAM)filePath.c_str()
      );

      // Send messege box to inform.
      MessageBox(
        hwnd,
        L"Đã lưu trang vẽ thành công!",
        L"Ê!",
        64
      );
    }

    catch (const std::underflow_error& e) {
      throw e;
    }

    catch (const std::exception& e) {
      throw e;
    }
  }

  /// <summary>
  /// Handle file export.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleFileExport(HWND hwnd) {
    try {
      // Get file destination.
      std::wstring filePath = FileDialog::exportFileDialog(hwnd);
      
      //
      // Basically we can't reuse the HBITMAP created 
      // as global (since we deleted everytime WM_PAINT end). 
      // Hence, we need to create and use a new one, locally.
      //

      // Calculate client area.
      HDC hdcScreen = GetDC(hwnd);

      // New screen buffer.
      // This will be passed to bitmap struct.
      HDC hdc = CreateCompatibleDC(hdcScreen);

      // Drawing content.
      HBITMAP hBMP = CreateCompatibleBitmap(
        hdcScreen,
        hClientRect.right - hClientRect.left,
        hClientRect.bottom - hClientRect.top - BUTTON_HEIGHT * 2
      );

      // Handling old bitmaps.
      HBITMAP oHBMP = (HBITMAP)SelectObject(hdc, hBMP);

      // Move current screen bits to the newly-created buffer.
      BitBlt(
        hdc,
        0, 0,
        hClientRect.right - hClientRect.left,
        hClientRect.bottom - hClientRect.top - BUTTON_HEIGHT * 2,
        hdcScreen,
        0, 0 + BUTTON_HEIGHT,
        SRCCOPY
      );

      // Create Bitmap info.
      PBITMAPINFO bitmapInfo = BitmapImage::createBitmapInfo(hwnd, hBMP);

      // Write to Bitmap.
      BitmapImage::createBitmapFile(
        hwnd,
        filePath,
        bitmapInfo,
        hBMP,
        hdc
      );

      // Revert back to old bitmap.
      SelectObject(hdc, oHBMP);

      // Clear allowcated memory.
      LocalFree((HLOCAL)bitmapInfo);
      DeleteDC(hdc);
      DeleteObject(hBMP);
      DeleteObject(oHBMP);
      ReleaseDC(hwnd, hdcScreen);

      // Informing users that exported succefully.
      MessageBox(
        hwnd,
        L"Xuất thành công ra file bitmap rồi đó!",
        L"Ê!",
        64
      );
    }

    catch (const std::underflow_error& e) {
      throw e;
    }

    catch (const std::exception& e) {
      throw e;
    }
  }

  /// <summary>
  /// Controller for file actions (new, open, save)
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="id"></param>
  void handleFileActions(HWND hwnd, int id) {
    try {
      switch (id) {
      case ID_FILE_NEW: 
      case ID_HOTKEY_NEW: {
        // Only ask user to save if the file is edited.
        if (programStatus & IS_CHANGED) {
          int notificationReturn = NotificationDialog::resetConfirmation(hwnd);

          // Break when user press Cancel.
          if (IDCANCEL == notificationReturn) {
            return;
          }

          // Trigger file saving when user press Yes.
          if (IDYES == notificationReturn) {
            handleFileSaveAs(hwnd);
          }
        }

        // Call controller handle this action.
        handleFileNew(hwnd);

        break;
      }
      case ID_FILE_OPEN: 
      case ID_HOTKEY_OPEN: {
        if (programStatus & IS_CHANGED) {
          int notificationReturn = NotificationDialog::resetConfirmation(hwnd);

          // Cancel hit.
          if (IDCANCEL == notificationReturn) {
            return;
          }

          // Yes hit.
          if (IDYES == notificationReturn) {
            handleFileSaveAs(hwnd);
          }
        }

        // Call controller handle file open.
        handleFileOpen(hwnd);

        break;
      }
      case ID_FILE_SAVE:
      case ID_HOTKEY_SAVE: {
        // Call controller handle file save.
        handleFileSaveAs(hwnd);

        break;
      }
      case ID_FILE_EXPORT: {
        // Call controller handle file export.
        handleFileExport(hwnd);
      }
      }
    }

    // Handle cancel (of most dialogs)
    catch (const std::underflow_error& e) {
      UNREFERENCED_PARAMETER(e);
      return;
    }

    // Someelse exception.
    catch (const std::exception& e) {
      MessageBoxA(
        hwnd,
        e.what(),
        "Error",
        MB_ICONERROR
      );
    }
  }
}

/// <summary>
/// Handling colour change.
/// </summary>
namespace ColourController {
  /// <summary>
  /// Controller handling changing background colour.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleChangeBackgroundColour(HWND hwnd) {
    try {
      COLORREF newColour = ColourDialog::chooseColourDialog(hwnd);
      defaultShapeGraphic.setBackgroundColour(newColour);
    }

    catch (const std::exception& e) {
      UNREFERENCED_PARAMETER(e);
      return;
    }
  }

  /// <summary>
  /// Handle changing line colour.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleChangeLineColour(HWND hwnd) {
    try {
      COLORREF newColour = ColourDialog::chooseColourDialog(hwnd);
      defaultShapeGraphic.setLineColour(newColour);
    }

    catch (const std::exception& e) {
      UNREFERENCED_PARAMETER(e);
      return;
    }
  }

  /// <summary>
  /// Controller for changing colour actions (line, background).
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="id"></param>
  void handleColourActions(HWND hwnd, int id) {
    switch (id) {
    case ID_COLOUR_BACKGROUND:
      handleChangeBackgroundColour(hwnd);
      break;

    case ID_COLOUR_LINE:
      handleChangeLineColour(hwnd);
      break;
    }
  }
}

/// <summary>
/// Handling pen change.
/// </summary>
namespace PenstyleController {
  /// <summary>
  /// First item of the pen menu.
  /// </summary>
  const int START_MENU_PEN_ITEM = ID_PENSTYLE_SOLID;

  /// <summary>
  /// Last item of the pen menu.
  /// </summary>
  const int END_MENU_PEN_ITEM = ID_PENSTYLE_DASH_DOT_DOT;

  /// <summary>
  /// Get penstyle from a selected menu item.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="menuId"></param>
  /// <returns>The defaultShapeGraphic.lineStyle() by default.</returns>
  int getPenFromMenuItem(HWND hwnd, int menuId = -1) {
    switch (menuId) {
    case ID_PENSTYLE_SOLID:
      return PS_SOLID;
    case ID_PENSTYLE_DASH:
      return PS_DASH;
    case ID_PENSTYLE_DOT:
      return PS_DOT;
    case ID_PENSTYLE_DASH_DOT:
      return PS_DASHDOT;
    case ID_PENSTYLE_DASH_DOT_DOT:
      return PS_DASHDOTDOT;
    }

    return defaultShapeGraphic.lineStyle();
  }

  /// <summary>
  /// Get menu item from a selected pen style.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="penID"></param>
  /// <returns></returns>
  int getMenuItemFromPen(HWND hwnd, int penID = -1) {
    switch (penID) {
    case PS_SOLID:
      return ID_PENSTYLE_SOLID;
    case PS_DASH:
      return ID_PENSTYLE_DASH;
    case PS_DOT:
      return ID_PENSTYLE_DOT;
    case PS_DASHDOT:
      return ID_PENSTYLE_DASH_DOT;
    case PS_DASHDOTDOT:
      return ID_PENSTYLE_DASH_DOT_DOT;
    }

    return getMenuItemFromPen(
      hwnd,
      defaultShapeGraphic.lineStyle()
    );
  }

  /// <summary>
  /// Check menu item.
  /// </summary>
  /// <param name="hwnd">Handle of the window</param>
  /// <param name="itemID">ID of the menu item</param>
  void checkMenuItem(HWND hwnd, int itemID) {
    // Get current active menu.
    HMENU hCurrentMenu = GetMenu(hwnd);

    // Check the item in that menu.
    CheckMenuRadioItem(
      hCurrentMenu,
      START_MENU_PEN_ITEM,
      END_MENU_PEN_ITEM,
      itemID,
      MF_BYCOMMAND
    );
  }

  /// <summary>
  /// Controller for changing pen style.
  /// </summary>
  /// <param name="hwnd">Handle of the window</param>
  /// <param name="id">Menu ID of the item selected</param>
  void handlePenstyleActions(HWND hwnd, int id) {
    // Get penstyle id.
    int cPenID = getPenFromMenuItem(hwnd, id);

    // Check that menu item.
    checkMenuItem(hwnd, id);

    // Change penstyle.
    defaultShapeGraphic.setLineStyle(
      cPenID
    );
  }
}

/// <summary>
/// Handling Toolbar actions.
/// </summary>
namespace ToolbarController {
  /// <summary>
  /// Create a toolbar
  /// </summary>
  /// <param name="hInst">Current instance</param>
  /// <param name="hwnd">Handle of current window</param>
  /// <param name="hToolbarWnd">Handle of current window's toolbar</param>
  void createToolbar(const HINSTANCE& hInst, const HWND& hwnd, HWND& hToolbarWnd) {
    // Declare buttons.
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
  }
}

/// <summary>
/// Handling statusbar actions.
/// </summary>
namespace StatusbarController {
  /// <summary>
  /// Buffer to store datas.
  /// </summary>
  wchar_t buffer[256];
  
  /// <summary>
  /// Create a status bar.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="hStatusBarWnd"></param>
  /// <param name="hClientRect"></param>
  void createStatusbar(const HWND& hwnd, 
    HWND& hStatusBarWnd, const RECT& hClientRect) {
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
  }

  /// <summary>
  /// Create statusbar text.
  /// </summary>
  /// <param name="label"></param>
  /// <param name="string"></param>
  void createText(const wchar_t* label, 
    const std::string& string) {
    std::wstring wString(string.begin(), string.end());

    wsprintfW(
      buffer,
      L"%s %s",
      label,
      wString.c_str()
    );
  }

  //
  // Methods defined inside this controller is critical,
  // since they require update on runtime.
  // 
  // Just add more things to finish it.
  //

  /// <summary>
  /// Update statusbar when creating shapes.
  /// </summary>
  /// <param name="hStatusBarWnd"></param>
  /// <param name="newShape"></param>
  void onCreateShape(HWND hStatusBarWnd, 
    const std::shared_ptr<IShape>& newShape) {
    createText(L"[Tạo mới]", newShape->toString());

    SendMessage(
      hStatusBarWnd,
      SB_SETTEXTW,
      0,
      (LPARAM)buffer
    );
  }

  /// <summary>
  /// Update statusbar when selecting shapes.
  /// </summary>
  /// <param name="hStatusBarWnd"></param>
  /// <param name="shape"></param>
  void onSelectShape(HWND hStatusBarWnd,
    const std::shared_ptr<IShape>& shape) {
    createText(
      L"[Chọn]",
      shape->toString()
    );

    SendMessage(
      hStatusBarWnd,
      SB_SETTEXTW,
      0,
      (LPARAM)buffer
    );
  }

  /// <summary>
  /// Update statusbar when moving shapes.
  /// </summary>
  /// <param name="hStatusBarWnd"></param>
  /// <param name="shape"></param>
  void onMoveShape(HWND hStatusBarWnd,
    const std::shared_ptr<IShape>& shape) {
    createText(
      L"[Di chuyển]",
      shape->toString()
    );

    SendMessage(
      hStatusBarWnd,
      SB_SETTEXTW,
      0,
      (LPARAM)buffer
    );
  }

  /// <summary>
  /// Track the mouse and record position.
  /// </summary>
  /// <param name="hStatusBarWnd"></param>
  /// <param name="x"></param>
  /// <param name="y"></param>
  void onMouseMove(HWND hStatusBarWnd,
    int x, int y) {
    wsprintf(buffer, L"Vị trí con trỏ: (%d, %d)", x, y);

    SendMessage(
      hStatusBarWnd,
      SB_SETTEXT,
      (WPARAM)1,
      (LPARAM)buffer
    );
  }
}

/// <summary>
/// Handling hotkey actions.
/// </summary>
namespace HotkeyController {
  /// <summary>
  /// Create hotkey for window.
  /// </summary>
  /// <param name="hwnd"></param>
  void createHotkey(HWND& hwnd) {
    // Hotkey for new workspace
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_NEW,
      MOD_CONTROL | MOD_NOREPEAT,
      0x4E
    );
    // Hotkey for open workspace
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_OPEN,
      MOD_CONTROL | MOD_NOREPEAT,
      0x4F
    );
    // Hotkey for save workspace
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_SAVE,
      MOD_CONTROL | MOD_NOREPEAT,
      0x53
    );
    // Hotkey for cut
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_CUT,
      MOD_CONTROL | MOD_NOREPEAT,
      0x58
    );
    // Hotkey for copy
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_COPY,
      MOD_CONTROL | MOD_NOREPEAT,
      0x43
    );
    // Hotkey for paste
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_PASTE,
      MOD_CONTROL | MOD_NOREPEAT,
      0x56
    );
    // Hotkey for remove.
    RegisterHotKey(
      hwnd,
      ID_HOTKEY_DELETE,
      MOD_NOREPEAT,
      VK_DELETE
    );
  }

  /// <summary>
  /// Destroy hotkey for window.
  /// </summary>
  /// <param name="hwnd"></param>
  void destroyHotkey(HWND& hwnd) {
    UnregisterHotKey(hwnd, ID_HOTKEY_COPY);
    UnregisterHotKey(hwnd, ID_HOTKEY_CUT);
    UnregisterHotKey(hwnd, ID_HOTKEY_PASTE);
    UnregisterHotKey(hwnd, ID_HOTKEY_NEW);
    UnregisterHotKey(hwnd, ID_HOTKEY_OPEN);
    UnregisterHotKey(hwnd, ID_HOTKEY_SAVE);
  }
}