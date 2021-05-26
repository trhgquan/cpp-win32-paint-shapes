#pragma once

/// <summary>
/// Notification (mostly messageboxes)
/// </summary>
namespace Notification {
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