#pragma once

/// <summary>
/// Functions related to a BITMAP.
/// </summary>
namespace BitmapImage {
  /// <summary>
  /// Create a bitmap header.
  /// Source: https://docs.microsoft.com/en-us/windows/win32/gdi/storing-an-image
  /// </summary>
  /// <param name="hwnd"></param>
  /// <param name="hBMP"></param>
  /// <returns></returns>
  PBITMAPINFO CreateBitmapInfo(HWND hwnd, HBITMAP hBMP) {
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;

    // Retrieve the bitmap colour format, width and height.
    GetObject(hwnd, sizeof(BITMAP), (LPSTR)&bmp);

    // Convert the colour format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1) {
      cClrBits = 1;
    }

    else if (cClrBits <= 4) {
      cClrBits = 4;
    }

    else if (cClrBits <= 8) {
      cClrBits = 8;
    }

    else if (cClrBits <= 16) {
      cClrBits = 16;
    }
      
    else if (cClrBits <= 24) {
      cClrBits = 24;
    }
      
    else {
      cClrBits = 32;
    }

    // Allocate memory for the BITMAPINFO structure.
    if (cClrBits < 24) {
      pbmi = (PBITMAPINFO)LocalAlloc(
        LPTR,
        sizeof(BITMAPINFOHEADER) +
        sizeof(RGBQUAD) + (1 << cClrBits)
      );
    }

    else {
      pbmi = (PBITMAPINFO)LocalAlloc(
        LPTR,
        sizeof(BITMAPINFOHEADER)
      );
    }

    // Initialise fields inside BITMAPINFO struct.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24) {
      pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
    }

    // If not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute number of bytes in the array of colour.
    pbmi->bmiHeader.biSizeImage = (
      (pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
      * pbmi->bmiHeader.biHeight;
    
    // Stating all colour is important.
    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;
  }

  
}