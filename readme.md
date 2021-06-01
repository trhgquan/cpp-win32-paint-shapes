# Paint - chương trình vẽ lung tung các đối tượng hình học.
Đồ án thực hành số 02 (cuối cùng) - Phương pháp Lập trình hướng đối tượng (CSC10003)

VNUHCM - University of Science, Hè 2021.

## Project này có thể:
1. Vẽ hình rất đẹp:
    - đường thẳng đẹp như đường thẳng,
    - hình chữ nhật đẹp như hình chữ nhật,
    - hình vuông đẹp như hình vuông,
    - hình ellipse đẹp như hình ellipse,
    - hình tròn đẹp như hình tròn.

2. Chọn mầu nền, mầu nét vẽ như họa sĩ thực thụ
3. Hỗ trợ 5 loại bút vẽ khác nhau:
    - Vẽ nét liền
    - Vẽ nét chấm
    - Vẽ nét đứt
    - Vẽ 2 nét đứt cách nhau 1 dấu chấm
    - Vẽ 2 nét đứt cách nhau 2 dấu chấm

4. Hỗ trợ lưu file, mở file bằng Win32 File Open / Save Dialog.
    - File save dạng text, tha hồ mà sửa (mở không lên được thì thôi).
5. Hỗ trợ export hình ra file bitmap `.bmp`.
## Cài
1. Clone project
```
git clone
```

2. Mở `Source/Paint.sln`.
3. Chuyển chế độ sang `Release`, sau đó `Build -> Rebuild Solution`.

## Ý tưởng:
### 1. Quá trình vẽ
- Mình lưu tất cả các shape đã vẽ vào 1 vector, sau khi thêm mới 1 shape
vào vector thì mình redraw lại toàn màn hình (dùng [InvalidateRect](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-invalidaterect))
- Với hình tròn và hình vuông, bắt buộc 2 điểm `topLeft` và `rightBottom` phải nằm
trên đường chéo của hình vuông. Bài toán đặt ra là phải ánh xạ `topLeft` và `rightBottom` về
2 điểm nằm trên cùng 1 đường chéo, và mình nghĩ nó khá đơn giản nếu bạn đọc code của mình 
`(Source/Library/Geometric.h)`.

### 2. Màu vẽ / nét vẽ
- Với mỗi shape, mình có một object `ShapeGraphic` lưu lại các thông số 
của shape (nét vẽ, màu vẽ, ..), cài luôn `ShapeGraphic::toString()` để có thể
in mấy cái thông số này ra dạng text. Mình đặt một global variable `ShapeGraphic`
luôn, mỗi lần vẽ 1 shape mới thì chỉ việc truyền global variable này vào constructor
là xong.
- Với mỗi lần đổi màu / nét, mình sẽ gán màu / nét mới vào global `ShapeGraphic` bên trên.

### 3. Mở / lưu workspace.
- Nếu bạn chú ý thì các file workspace có đuôi `.txt` và có nội dung khá dễ nhìn. 
  Mục đích chính là để parse cho dễ, hàm parse sử dụng `Source/Library/Tokeniser.h`; `Shape.parse()` được cài trong tất cả các shape. Tương tự với hàm `Shape.toString()` convert một shape sang `std::string` để ghi vào file.
- __Còn tại sao mình xài `std::string`, là vì mình lười chuyển sang binary và mình thấy
  chuyển sang binary nó tốn nhiều nước mắt và trầm cảm quá!__ (nhưng bạn có thể tự chuyển
  sang binary, nhỉ?)

### 4. Export ra bitmap.
- Mình dùng [PrintWindow](https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-printwindow) để capture client area, sau đó tạo file từ struct `HBITMAP` vừa capture.
- Tài liệu tham khảo về cách tạo bitmap header và tạo file bitmap từ struct` HBITMAP`: 
  [Microsoft Win32 docs](https://docs.microsoft.com/vi-vn/windows/win32/gdi/storing-an-image?redirectedfrom=MSDN)
## Quan trọng
- Đồ họa của project không xấu, đó là do tài năng hội họa của bạn chưa chín muồi!

