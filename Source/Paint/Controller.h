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
  }

  /// <summary>
  /// Cut a shape,
  /// basically this point the selected to the last
  /// and remove it from the vector.
  /// </summary>
  /// <param name="hwnd"></param>
  void cutShapeDrawing(HWND hwnd) {
    copyShapeDrawing(hwnd);

    removeShapeDrawing(hwnd);
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
      break;
    }
    case ID_SHAPE_SELECT: {
      programStatus = IS_SELECTING;
      break;
    }
    case ID_SHAPE_DELETE:
    case ID_EDITMENU_DELETE: {
      if (shapesVector.size() > 0) {
        // User needs to confirming the process before going further.
        int confirmation = NotificationDialog::deleteComfirmation(hwnd);

        if (IDYES == confirmation) {
          removeShapeDrawing(hwnd);
        }
      }
      break;
    }
    case ID_SHAPE_COPY:
    case ID_EDITMENU_COPY: {
      copyShapeDrawing(hwnd);
      break;
    }
    case ID_SHAPE_CUT:
    case ID_EDITMENU_CUT: {
      cutShapeDrawing(hwnd);
      break;
    }
    case ID_SHAPE_PASTE:
    case ID_EDITMENU_PASTE: {
      pasteShapeDrawing(hwnd);
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
      case ID_FILE_NEW: {
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
      case ID_FILE_OPEN: {
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
      case ID_FILE_SAVE: {
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
/// Handling statusbar actions.
/// </summary>
namespace StatusbarController {
  /// <summary>
  /// Buffer to store datas.
  /// </summary>
  wchar_t buffer[256];

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