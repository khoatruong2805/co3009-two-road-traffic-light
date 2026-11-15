# CO3009 · Đèn giao thông hai đường

Firmware cho XNUCLEO-F103RB: hai đường luân phiên xanh/vàng/đỏ, điều khiển tay, chỉnh thời gian, đọc DHT20 và hiển thị LCD 16×2. Nhánh `dev` dành cho nhóm tích hợp; `main` bắt đầu từ cùng bản nền. Commit khởi tạo là mốc chung, không tính vào đóng góp cá nhân. Chưa nạp board, chưa phải bản nộp cuối.

| Mục đích | Mở ở đâu |
|---|---|
| Import project thực | [TrafficRB](TrafficRB) – CubeIDE, MCU STM32F103RBTx |
| Mô phỏng có thể chạy ngay | [TrafficC6.pdsprj](simulation/TrafficC6.pdsprj) – Proteus 8.10; giữ nguyên thư mục [firmware](simulation/firmware) |
| Schematic assignment | [Assignment.pdsprj](simulation/Assignment.pdsprj) – bản sao đã sửa rail 3,3 V, còn thiếu hai model VSM |
| Chạy lại test và xem kết quả | [Kiểm thử](docs/verification.md) |
| Phân công, feature branch, PR | [Quy trình nhóm](CONTRIBUTING.md) |
| Phần thân báo cáo | [report/body.tex](report/body.tex) – chèn vào khung LaTeX hiện có |

## Build, flash và đo trên board

1. Clone `https://github.com/khoatruong2805/co3009-two-road-traffic-light.git`. Trong STM32CubeIDE chọn **File → Import → General → Existing Projects into Workspace** và chọn thư mục `TrafficRB`. Mở `TrafficRB.ioc`: board **NUCLEO-F103RB**, SYS → Debug **Serial Wire**. Đã có code sinh và HAL/CMSIS; không cần Generate Code để build. Nếu sinh lại từ .ioc, giữ nguyên vùng USER CODE.
2. **Project → Build Project** (Debug). Sản phẩm: `TrafficRB/Debug/TrafficRB.elf`, `.hex`, `.bin`. Đã build với CubeIDE 1.7.0, FW_F1 V1.8.7, ARM GCC 9.3.1. Không nạp file của mạch C6 vào board.
3. **Khi nhóm có board:** đặt `BOARD_BRINGUP=1` trong `Core/Inc/board_config.h`, build, kết nối ST-LINK qua USB; trong Run/Debug chọn **STM32 C/C++ Application**, ST-LINK/SWD, ELF của TrafficRB. Đây là chương trình đo trên bàn; nó lần lượt xuất cả bốn mã trên cả hai module, không điều khiển giao thông.
4. Xem Live Expressions: `diag_ab` (00…11, giữ mỗi mã 2 s), `diag_buttons_raw` (bit 0…3), `diag_user_raw`, `diag_i2c_count`, `diag_i2c_addr`. PA5/PC9 đảo mức mỗi 0,5/1 s. Đợi khoảng 2 s để quét xong; địa chỉ là **7 bit**. Ghi kết quả cho từng module; sửa bảng màu, mặt nạ nút, địa chỉ và wiring backpack nếu cần.
5. Đặt lại `BOARD_BRINGUP=0`, build và flash lại; kiểm tra hai hướng, thao tác nút và LCD. Nếu DHT20 không trả bản tin hợp lệ, LCD hiển thị dấu gạch. Timing áp dụng trong RAM; reset về mặc định.

## Nối dây và sử dụng

| Grove/header | STM32 | Ý nghĩa |
|---|---|---|
| D3, D4 | PB3, PB5 | TL1_A, TL1_B |
| D5, D6 | PB4, PB10 | TL2_A, TL2_B |
| D7, D8 | PA8, PA9 | MODE, NEXT |
| D9, D10 | PC7, PB6 | PLUS, MINUS |
| SCL, SDA | PB8, PB9 | I2C1 remap, 100 kHz; DHT20 0x38; LCD mặc định 0x27 |
| SWDIO, SWCLK | PA13, PA14 | Dành riêng cho debug; JTAG tắt để dùng PB3/PB4 |

**Auto:** xanh 1/đỏ 2 trong 5 s → vàng 1/đỏ 2 trong 2 s → đỏ 1/xanh 2 trong 5 s → đỏ 1/vàng 2 trong 2 s. **MODE** vào Manual ở đỏ/đỏ; **NEXT** tiến một pha. **MODE** lần nữa vào Tuning ở đỏ/đỏ. **NEXT** chọn đỏ/xanh/vàng, **PLUS/MINUS** đổi 1 s (1…99). **MODE** xác nhận và về Auto khi đỏ = xanh + vàng; nếu sai LCD báo lỗi và giữ đỏ/đỏ. Hàng hai LCD hiện nhiệt độ/độ ẩm, trừ lúc cần báo lỗi thời gian.

## Proteus

Mở `simulation/TrafficC6.pdsprj` rồi bấm **Run (F12)**. Nếu di chuyển project, Program File của U1 là `firmware/traffic_demo.hex` tương đối với file mạch. Mạch dùng sáu LED mức thấp và ba nút; BT0=MODE, BT1=NEXT, BT2 nhấn ngắn rồi thả=PLUS, giữ ≥0,8 s rồi thả=MINUS. Bốn LED 7 đoạn lần lượt là mode (1/2/3), mục chỉnh (1/2/3 hoặc 0), hai chữ số giá trị (số pha hoặc thời gian đang chỉnh). C6 dùng chung scheduler và các FSM với firmware board; không mô phỏng DHT20/LCD.

File HEX nhỏ của **mạch demo** được giữ trong repo để có thể mở ngay. Để tạo lại: `python simulation/build_sim.py` nếu ARM GCC nằm trong PATH, hoặc `python simulation/build_sim.py --bin "<thư mục bin của GNU Tools for STM32>"`.

## Báo cáo và nộp

Trong file chính của khung báo cáo, chèn `\input{report/body}` ở phần nội dung (điều chỉnh đường dẫn nếu cần); dùng bộ gõ/phông tiếng Việt của khung. Nội dung bắt đầu bằng `\section`, không cần bìa mới hay TikZ. Trên bìa nhóm tự điền họ tên, MSSV, nhóm, URL repo và mã commit đã chốt. Hạn theo đề: **04/12/2026**. Quy tắc đặt tên PDF ở A4 và Bước 7 khác nhau; đối chiếu LMS trước khi nộp. Nhóm chỉ gắn tag cuối sau khi kiểm tra board và chốt kết quả thực tế.
