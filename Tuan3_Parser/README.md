# KPL Parser (IT3323 - Chương trình dịch)

**Họ và tên**: Tô Minh Ánh

**MSSV**: 20225787

## 🚀 Tính năng đã hoàn thiện

Dự án đã hoàn thành các bài tập theo yêu cầu:

1.  **Bài tập 1.1:** Phân tích các khai báo (Declarations)
    * Khai báo hằng (`CONST`)
    * Khai báo kiểu (`TYPE`)
    * Khai báo biến (`VAR`)
    * Khai báo chương trình con (`FUNCTION`, `PROCEDURE`)

2.  **Bài tập 1.2:** Phân tích câu lệnh và biểu thức (Statements & Expressions)
    * Câu lệnh gán (`:=`)
    * Câu lệnh gọi hàm/thủ tục (`CALL`)
    * Khối lệnh (`BEGIN ... END`)
    * Câu lệnh điều kiện (`IF ... THEN ... ELSE`)
    * Vòng lặp (`WHILE ... DO`, `FOR ... TO ... DO`)
    * Xử lý biểu thức toán học và logic đầy đủ.

3.  **Bài tập 1.3:** Tích hợp toàn hệ thống
    * Chạy thành công với các chương trình phức tạp (Mảng, Hàm đệ quy, v.v.).

4.  **Bài tập 2:**
    * **Vòng lặp REPEAT:** Hỗ trợ cú pháp `REPEAT <statement> UNTIL <condition>`.
    * **Gán đa biến (Parallel Assignment):** Hỗ trợ cú pháp `x, y, z := 1, 2, 3`.

## 🛠️ Hướng dẫn Biên dịch (Build)

Bạn có thể biên dịch dự án bằng `make` hoặc `gcc` thủ công.

### Cách 1: Sử dụng Make (Khuyên dùng)
Nếu đã cài đặt Make (MinGW/MSYS2):

```bash
# Xóa các file dịch cũ (quan trọng khi sửa token.h)
make clean

# Biên dịch chương trình
make
```
### Cách 2: Sử dụng GCC thủ công

```bash
gcc main.c parser.c scanner.c reader.c charcode.c token.c error.c -o parser
```

## ▶️ Hướng dẫn Chạy (Run)

Sau khi biên dịch thành công, file thực thi `parser.exe` (Windows) hoặc `parser` (Linux/Mac) sẽ được tạo ra.

Cú pháp chạy:

```bash
./parser <tên_file_nguồn.kpl>
```

## 📂 Cấu trúc thư mục

  * `main.c`: Chương trình chính, đọc tham số dòng lệnh.
  * `parser.c/h`: Bộ phân tích cú pháp (Core logic).
  * `scanner.c/h`: Bộ phân tích từ vựng (Lexical Analyzer).
  * `token.c/h`: Định nghĩa các Token và từ khóa.
  * `reader.c/h`: Đọc file nguồn và xử lý dòng/cột.
  * `error.c/h`: Quản lý và thông báo lỗi.
  * `charcode.c/h`: Bảng mã ký tự.
  * `Makefile`: Script tự động biên dịch.

**Lưu ý:** Hãy chắc chắn chạy `make clean` rồi `make` để biên dịch lại toàn bộ dự án.