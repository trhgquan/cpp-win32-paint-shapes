#pragma once

/// <summary>
/// File Dialogs
/// </summary>
namespace FileDialog {
  /// <summary>
  /// Open a file open dialog for file chosen.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns>Path to the chosen file.</returns>
  std::wstring openFileDialog(HWND hwnd) {
    // Temp for file name.
    TCHAR szOpenFile[MAX_PATH + 1];

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

    // Throw error if dialog canceled.
    throw std::underflow_error(
      "(FileOpenDialog) Dialog canceled."
    );
  }

  /// <summary>
  /// Open a file save dialog for file save.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns>Path to the save file.</returns>
  std::wstring saveFileDialog(HWND hwnd) {
    // Temp for file save path
    TCHAR szSaveFile[MAX_PATH + 1];

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

    // Throw std::underflow if user cancelled.
    throw std::underflow_error(
      "(SaveFileDialog) Dialog canceled"
    );
  }
}

/// <summary>
/// Colour dialogs
/// </summary>
namespace ColourDialog {
  /// <summary>
  /// Open choose colour dialog.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns></returns>
  COLORREF chooseColourDialog(HWND hwnd) {
    COLORREF chosenColour[16];

    ZeroMemory(&hChooseColour, sizeof(hChooseColour));
    hChooseColour.lStructSize = sizeof(hChooseColour);
    hChooseColour.hwndOwner = hwnd;
    hChooseColour.lpCustColors = (LPDWORD)chosenColour;
    hChooseColour.rgbResult = RGB(0, 0, 0);
    hChooseColour.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&hChooseColour)) {
      return hChooseColour.rgbResult;
    }

    throw std::exception();
  }
}

/// <summary>
/// Notification (mostly messageboxes)
/// </summary>
namespace NotificationDialog {
  /// <summary>
  /// Ask if user want to save before continue.
  /// </summary>
  /// <param name="hwnd"></param>
  /// <returns></returns>
  int resetConfirmation(HWND hwnd) {
    int messageReturn = MessageBox(
      hwnd,
      L"Nội dung nãy giờ có thay đổi, giờ mà bỏ đi là coi như xong ấy. Ông có muốn lưu lại không?",
      L"Ê!",
      MB_YESNOCANCEL | MB_ICONINFORMATION
    );

    return messageReturn;
  }
}