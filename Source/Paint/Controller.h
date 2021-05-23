#pragma once

/// <summary>
/// Controller - handle special actions
/// </summary>
namespace Controller {
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
      
      // Reset untouch status.
      hasChanged = false;

      // Reset all shapes.
      shapesVector.clear();

      // Call for clear screen.
      InvalidateRect(hwnd, NULL, true);

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

      // Clear the old shape vector.
      shapesVector.clear();

      // Reset untouch status
      hasChanged = false;

      // Open file stream
      std::ifstream in("shapes.txt");
      std::string buffer;

      while (std::getline(in, buffer)) {
        std::vector<std::string> tokens = Tokeniser::split(buffer, ": ");
        shapesVector.push_back(
          shapeFactory->parse(tokens[0], tokens[1])
        );
      }

      in.close();

      // Call for clear screen and redraw.
      InvalidateRect(hwnd, NULL, true);
      break;
    }
    case ID_FILE_SAVE: {
      std::ofstream out("shapes.txt");

      for (int i = 0; i < shapesVector.size(); ++i) {
        out << shapesVector[i]->toString() << '\n';
      }

      out.close();

      MessageBox(hwnd, L"File saved.", L"Notification", 64);
    }
    }
  }
}