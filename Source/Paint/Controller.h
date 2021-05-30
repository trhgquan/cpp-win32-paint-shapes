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
    hasChanged = false;

    // The screen is now undraw.
    isDrawing = false;

    // Reset all shapes.
    shapesVector.clear();
  }

  /// <summary>
  /// Handle shape-changing (change to another shape).
  /// </summary>
  /// <param name="id"></param>
  void handleShapeChanging(int id) {
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

    MessageBox(hwnd,
      L"Mới tạo workspace mới ấy!",
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

      // Send message box informs that user has opened the file sucessfully.
      MessageBox(hwnd, L"Đã load file thành công", L"Ê!", 64);
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
  void handleFileSave(HWND hwnd) {
    try {
      std::wstring filePath = FileDialog::saveFileDialog(hwnd);
      std::ofstream out(filePath);

      for (int i = 0; i < shapesVector.size(); ++i) {
        out << shapesVector[i]->toString() << '\n';
      }

      out.close();

      MessageBox(hwnd, L"Đã save file thành công!", L"Ê!", 64);
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
        if (hasChanged) {
          int notificationReturn = NotificationDialog::resetConfirmation(hwnd);

          // Break when user press Cancel.
          if (IDCANCEL == notificationReturn) {
            return;
          }

          // Trigger file saving when user press Yes.
          if (IDYES == notificationReturn) {
            handleFileSave(hwnd);
          }
        }

        // Call controller handle this action.
        handleFileNew(hwnd);

        break;
      }
      case ID_FILE_OPEN: {
        if (hasChanged) {
          int notificationReturn = NotificationDialog::resetConfirmation(hwnd);

          // Cancel hit.
          if (IDCANCEL == notificationReturn) {
            return;
          }

          // Yes hit.
          if (IDYES == notificationReturn) {
            handleFileSave(hwnd);
          }
        }

        // Call controller handle file open.
        handleFileOpen(hwnd);

        break;
      }
      case ID_FILE_SAVE: {
        // Call controller handle file save.
        handleFileSave(hwnd);
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