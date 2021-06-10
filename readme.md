# Paint - chương trình vẽ lung tung các đối tượng hình học.
[![MSBuild](https://github.com/trhgquan/cpp-win32-paint-shapes/actions/workflows/msbuild.yml/badge.svg?branch=main)](https://github.com/trhgquan/cpp-win32-paint-shapes/actions/workflows/msbuild.yml)

Lab Project 02 - Phương pháp Lập trình hướng đối tượng (CSC10003).

__GVHD:__ Thầy Trần Duy Quang.

__Sinh viên thực hiện:__ Trần Hoàng Quân - __MSSV:__ 19120338.

VNUHCM - University of Science, Hè 2021. 

_Được làm từ C/C++ với Win32 API, với Visual Studio 2019, cùng nước mắt và sự trầm cảm._

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

4. Cắt, chép, dán, xóa một cách luxury vip pro siêu cấp vô địch vũ trụ.

5. Hỗ trợ lưu file, mở file bằng Win32 File Open / Save Dialog.
    - File save dạng text, tha hồ mà sửa (mở không lên được thì thôi).

6. Hỗ trợ phím tắt cho một số tính năng!
    - Xóa.
    - Cắt, chép và dán.
    - Mở, lưu và tạo trang vẽ mới.

7. Hỗ trợ export hình ra file bitmap `.bmp`.
## Cài
1. Clone project
```
git clone
```

2. Mở `Source/Paint.sln`.
3. Chuyển chế độ sang `Release`, sau đó `Build -> Rebuild Solution`.

## Hướng dẫn sử dụng
Xem [hướng dẫn sử dụng ở wiki](https://github.com/trhgquan/cpp-win32-paint-shapes/wiki).

## Facts
- Đồ họa của project không xấu, đó là do tài năng hội họa của bạn. Đừng đổ thừa!
- Icon trên toolbar __rất xấu__ là vì thằng dev đã vẽ nó bằng tay trên MS Paint!
 
## Tri ân
Những website tuyệt vời này đã hỗ trợ, đồng hành cùng mình vượt qua quá trình dev
đầy đau khổ và nước mắt:
- [Website hướng dẫn của thầy Trần Duy Quang](https://c4w.herokuapp.com)
- StackOverflow (MVP!)
- [Microsoft Win32 documentation](https://docs.microsoft.com/en-us/windows/win32/).

Project có sử dụng một số tài nguyên rất tuyệt vời:
- `pepethefrog.cur` - [credit](https://github.com/trhgquan/cpp-win32-paint-shapes/tree/main/Source/Paint/assets/cursor)
- `Pepe favicon` - [credit](https://github.com/trhgquan/cpp-win32-paint-shapes/tree/main/Source/Paint/assets/icon)