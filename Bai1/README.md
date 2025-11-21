# Text Indexer - Chương Trình Tạo Chỉ Mục Văn Bản

Chương trình này được viết bằng ngôn ngữ C nhằm mục đích đọc một tệp văn bản tiếng Anh và tạo ra một **bảng chỉ mục (Index Table)**. [cite\_start]Bảng chỉ mục liệt kê các từ xuất hiện trong văn bản theo thứ tự từ điển, kèm theo số lần xuất hiện và danh sách các dòng mà từ đó xuất hiện.

## 🚀 Tính năng

  * **Thống kê từ:** Đếm số lần xuất hiện và ghi lại số dòng của mỗi từ.
  * **Chuẩn hóa:** Chuyển đổi tất cả các từ về chữ thường (case-insensitive) để không phân biệt hoa thường].
  * **Lọc từ dừng (Stop Words):** Tự động loại bỏ các từ không có ý nghĩa tra cứu (như *a, an, the, in, on...*) dựa trên tệp cấu hình `stopw.txt` .
  * **Lọc danh từ riêng:** Tự động loại bỏ các danh từ riêng (các từ viết hoa chữ cái đầu nhưng không đứng sau dấu chấm câu).
  * **Sắp xếp:** Kết quả đầu ra được sắp xếp theo thứ tự từ điển (A-Z).
  * **Định dạng đầu ra:** `từ  số_lượng,dòng_1,dòng_2,...`.

## 📂 Cấu trúc Thư mục

Đảm bảo thư mục dự án của bạn có các tệp sau:

```text
.
├── indexer.c       # Mã nguồn chính của chương trình
├── stopw.txt       # Tệp chứa danh sách các từ dừng (mỗi từ một dòng)
├── vanban.txt      # Tệp văn bản đầu vào cần xử lý
└── README.md       # Tài liệu hướng dẫn sử dụng
```

> **Lưu ý:** Sau khi chạy chương trình, tệp `output.txt` sẽ được tạo ra chứa kết quả.

## 🛠️ Yêu cầu hệ thống

  * **Trình biên dịch:** GCC (GNU Compiler Collection) hoặc bất kỳ trình biên dịch C chuẩn nào.
  * **Hệ điều hành:** Linux, macOS, hoặc Windows (khuyến nghị dùng MinGW hoặc WSL).

## ⚙️ Hướng dẫn Biên dịch và Chạy

### 1\. Biên dịch

Mở terminal (hoặc CMD) tại thư mục chứa mã nguồn và chạy lệnh sau:

```bash
gcc indexer.c -o indexer
```

*(Lệnh này sẽ tạo ra tệp thực thi tên là `indexer` trên Linux/Mac hoặc `indexer.exe` trên Windows)*

### 2\. Chạy chương trình

Chạy tệp thực thi vừa tạo:

**Trên Linux/macOS:**

```bash
./indexer
```

**Trên Windows:**

```cmd
indexer.exe
```

### 3\. Kiểm tra kết quả

Sau khi chương trình chạy xong, mở tệp `output.txt` được tạo ra trong cùng thư mục để xem bảng chỉ mục.

## 📝 Định dạng Dữ liệu

### Tệp đầu vào (`stopw.txt`)

Chứa các từ cần loại bỏ, mỗi từ nằm trên một dòng. Ví dụ:

```text
a
an
the
of
...
```

### Tệp đầu ra (`output.txt`)

Kết quả được ghi theo định dạng: **`từ <số lần xuất hiện>,<dòng 1>,<dòng 2>...`**

Ví dụ:

```text
answer 4,7,8,12,15
ant 3,2,4,6
baby 3,7,9,21
cruel 3,2,4,5
```

## 💡 Giải thích Thuật toán (Logic xử lý)

1.  **Xử lý Stop Words:** Chương trình đọc `stopw.txt` vào bộ nhớ và sắp xếp để dùng thuật toán tìm kiếm nhị phân (Binary Search) giúp tra cứu nhanh.
2.  **Xử lý Danh từ riêng (Proper Noun):**
      * Chương trình theo dõi trạng thái "sau dấu câu" (`afterPunctuation`).
      * Nếu một từ bắt đầu bằng chữ in hoa (`A-Z`) và **không** nằm ngay sau dấu kết thúc câu (`.`, `?`, `!`) hoặc xuống dòng (`\n`), từ đó được coi là danh từ riêng và bị loại bỏ.
      * Trường hợp đặc biệt: `Hello. World`. Từ "World" nằm sau dấu chấm nên không bị coi là danh từ riêng và vẫn được đưa vào chỉ mục.
3.  **Lưu trữ & Sắp xếp:**
      * Các từ hợp lệ được thêm vào mảng động.
      * Nếu từ đã tồn tại, chương trình tăng biến đếm (`count`) và nối thêm số dòng vào chuỗi ký tự `lines`.
      * Cuối cùng, sử dụng `qsort` để sắp xếp toàn bộ danh sách theo bảng chữ cái[cite: 18, 51].

## 🔧 Tùy chỉnh

Hiện tại tên tệp đầu vào đang được gán cứng là `vanban.txt`. Để xử lý tệp khác (ví dụ `alice30.txt`), bạn hãy mở file `indexer.c` và sửa dòng sau trong hàm `main`:

```c
const char* inputFile = "alice30.txt"; // Đổi tên file tại đây
```

Sau đó biên dịch và chạy lại.
