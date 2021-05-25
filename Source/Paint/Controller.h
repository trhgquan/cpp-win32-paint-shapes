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
  /// Open a file open dialog for file chosen.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns>Path to the chosen file.</returns>
  std::wstring openFileDialog(HWND hwnd) {
    // Temp for file name.
    TCHAR szOpenFile[256 * MAX_LOADSTRING];

    // Initialise for file open dialog.
    ZeroMemory(&hOpenFile, sizeof(hOpenFile));
    hOpenFile.lStructSize = sizeof(hOpenFile);
    hOpenFile.hwndOwner = hwnd;
    hOpenFile.lpstrFile = szOpenFile;
    hOpenFile.lpstrFile[0] = '\0';
    hOpenFile.nMaxFile = sizeof(szOpenFile);
    hOpenFile.lpstrFilter = L"Text (*.txt)\0*.txt\0";
    hOpenFile.nFilterIndex = 1;
    hOpenFile.lpstrFileTitle = NULL;
    hOpenFile.nMaxFileTitle = 0;
    hOpenFile.lpstrInitialDir = NULL;
    hOpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Call file open dialog
    if (GetOpenFileName(&hOpenFile)) {
      // Convert file name to wstring.
      std::wstring fileName = hOpenFile.lpstrFile;

      return fileName;
    }
    
    // throw error if user not choosing anything.
    throw std::exception();
  }

  /// <summary>
  /// Open a file save dialog for file save.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns>Path to the save file.</returns>
  std::wstring saveFileDialog(HWND hwnd) {
    // Temp for file save path
    TCHAR szSaveFile[256 * MAX_LOADSTRING];

    // Initialise for file save dialog.
    ZeroMemory(&hSaveFile, sizeof(hSaveFile));
    hSaveFile.lStructSize = sizeof(hSaveFile);
    hSaveFile.hwndOwner = hwnd;
    hSaveFile.lpstrFile = szSaveFile;
    hSaveFile.lpstrFile[0] = '\0';
    hSaveFile.nMaxFile = sizeof(szSaveFile);
    hSaveFile.lpstrFilter = L"Text (*.txt)\0*.txt\0";
    hSaveFile.lpstrDefExt = L"txt";
    hSaveFile.nFilterIndex = 1;
    hSaveFile.lpstrFileTitle = NULL;
    hSaveFile.nMaxFileTitle = 0;
    hSaveFile.lpstrInitialDir = NULL;
    hSaveFile.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Call file save dialog
    if (GetSaveFileName(&hSaveFile)) {
      std::wstring fileName = hSaveFile.lpstrFile;

      return fileName;
    }

    throw std::exception();
  }

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
  }

  /// <summary>
  /// Open FileOpenDialog, user choose a file,
  /// then load shapes into vectors and redraw the screen.
  /// </summary>
  /// <param name="hwnd"></param>
  void handleFileOpen(HWND hwnd) {
    try {
      // Open file dialog and get file path.
      std::wstring filePath = openFileDialog(hwnd);
      std::ifstream in(filePath.c_str());
      std::string buffer;

      // Clear all shapes on screen and in vectors.
      ShapeController::resetShapeDrawing(hwnd);

      // Read shapes to a vector.
      while (std::getline(in, buffer)) {
        std::vector<std::string>tokens = Tokeniser::split(buffer, ": ");
        shapesVector.push_back(shapeFactory->parse(
          tokens[0],
          tokens[1]
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
      std::wstring filePath = saveFileDialog(hwnd);
      std::ofstream out(filePath);

      for (int i = 0; i < shapesVector.size(); ++i) {
        out << shapesVector[i]->toString() << '\n';
      }

      out.close();

      MessageBox(hwnd, L"Đã save file thành công!", L"Ê!", 64);
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
    switch (id) {
    case ID_FILE_NEW: {
      // Only ask user to save if the file is edited.
      if (hasChanged) {
        // Break when user press Cancel.
        if (!Notification::resetConfirmation(hwnd)) {
          return;
        }
      }

      // Call controller handle this action.
      handleFileNew(hwnd);

      MessageBox(hwnd,
        L"Mới tạo workspace mới ấy!",
        L"Ê!",
        MB_ICONINFORMATION
      );

      break;
    }
    case ID_FILE_OPEN: {
      if (hasChanged) {
        if (!Notification::resetConfirmation(hwnd)) {
          return;
        }
      }

      try {
        // Call controller handle file open.
        handleFileOpen(hwnd);
      }

      catch (const std::exception& e) {
        MessageBox(
          hwnd, 
          L"Đã có lỗi trong quá trình mở file.\nThử check lại đường dẫn / nội dung file và thử lại!", 
          L"Ê! Lỗi!", 
          16
        );
      }

      break;
    }
    case ID_FILE_SAVE: {
      try {
        // Call controller handle file save.
        handleFileSave(hwnd);
      }

      catch (const std::exception& e) {
      MessageBox(
        hwnd, 
        L"Đã có lỗi trong quá trình lưu!\nCoi lại coi có đặt tên tầm bậy hay chưa hỏi quyền admin từ vợ không?", 
        L"Ê! Lỗi!", 
        16
      );
      }
    }
    }
  }
}