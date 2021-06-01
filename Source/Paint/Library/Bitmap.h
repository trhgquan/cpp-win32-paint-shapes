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
  PBITMAPINFO createBitmapInfo(HWND hwnd, HBITMAP hBMP) {
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;

    // Retrieve the bitmap colour format, width and height.
    if (!GetObject(hBMP, sizeof(BITMAP), (LPSTR)&bmp)) {
      throw std::exception(
        "(CreateBitmapInfo) GetObject"
      );
    }

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
        sizeof(RGBQUAD) + (uint64_t(1) << cClrBits)
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

  /// <summary>
  /// Create a bitmap file.
  /// Source: https://docs.microsoft.com/en-us/windows/win32/gdi/storing-an-image
  /// </summary>
  /// <param name="hwnd">Handle of the window.</param>
  /// <param name="pszFile">Path to the destination file</param>
  /// <param name="pbi">BITMAPINFO</param>
  /// <param name="hBMP">HBITMAP</param>
  /// <param name="hdc">HDC</param>
  void createBitmapFile(HWND hwnd, const std::wstring& pszFile,
    PBITMAPINFO pbi, HBITMAP hBMP, HDC hdc) {
    HANDLE hf;
    BITMAPFILEHEADER hdr;
    PBITMAPINFOHEADER pbih;
    LPBYTE lpBits;
    DWORD dwTotal;
    DWORD cb;
    BYTE* hp;
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) {
      throw std::exception(
        "(createBitmapFile) GlobalAlloc"
      );
    }

    if (!GetDIBits(hdc, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
      DIB_RGB_COLORS)) {
      throw std::exception(
        "(createBitmapFile) GetDIBits"
      );
    }

    // Create handle to the file.
    hf = CreateFile(pszFile.c_str(),
      GENERIC_READ | GENERIC_WRITE,
      (DWORD)0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      (HANDLE)NULL);

    if (INVALID_HANDLE_VALUE == hf) {
      throw std::exception(
        "(createBitmapFile) CreateFile"
      );
    }

    hdr.bfType = 0x4d42;           // "BM"
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
      pbih->biSize + pbih->biClrUsed *
      sizeof(RGBQUAD) + pbih->biSizeImage);

    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
      pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

    // Write BITMAPFILEHEADER to the file.
    if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
      (LPDWORD)&dwTmp, NULL)) {
      throw std::exception(
        "(createBitmapFile) WriteFile - BITMAPFILEHEADER"
      );
    }

    // Write BITMAPINFOHEADER to the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
      + pbih->biClrUsed * sizeof(RGBQUAD),
      (LPDWORD)&dwTmp, NULL)) {
      throw std::exception(
        "(createBitmapFile) WriteFile - BITMAPINFOHEADER"
      );
    }

    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;

    // Write bmp file array.
    if (!WriteFile(hf, (LPSTR) hp, (int)cb, (LPDWORD)&dwTmp, NULL)) {
      throw std::exception(
        "(createBitmapFile) WriteFile - "
      );
    }

    // Close file handle
    CloseHandle(hf);

    // Free allocated bits.
    GlobalFree((HGLOBAL)lpBits);
  }
}