# 🐚 MyShell — Simple Yet Powerful Windows Shell

<div align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=24&duration=3000&pause=1000&color=58A6FF&center=true&vCenter=true&width=450&lines=A+custom+shell+in+C%2B%2B+for+Windows!">
</div>

---

## 🎯 Giới thiệu

**MyShell** là một trình mô phỏng terminal đơn giản được viết hoàn toàn bằng C++, chạy trên hệ điều hành Windows. Dự án không chỉ hỗ trợ các lệnh cơ bản như quản lý tệp, thư mục, tiến trình... mà còn có:

- Hệ thống **built-in commands**
- Hỗ trợ chạy **tiến trình foreground/background**
- Hệ **màu tùy chỉnh console** thông minh
- **Script runner** tự động hóa hành vi shell
- Các tiện ích vui vẻ như `tictactoe`, `spinner`, `logger`...

> 💡 Đây là một dự án thực hành trong môn *Nguyên lý Hệ điều hành* 

---

## 🧑‍💻 Thành viên phát triển

- Đỗ Trường Giang
- Nguyễn Thanh Tú
- Hoàng Minh Khôi
- Phạm Bảo Yến

---

## 🛠️ Yêu cầu hệ thống

- Windows 10/11
- C++17 trở lên
- Compiler hỗ trợ WinAPI (MSVC hoặc MinGW)
- CMake >= 3.10

---

## ⚙️ Build và chạy

```bash
git clone https://github.com/khoihoangminh05/MyShell.git
cd MyShell
mkdir build && cd build
cmake .. 
mingw32-make 
.\MyShell
