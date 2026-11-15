# Minh · Board, nút và chuyển mode

Đối chiếu từng chân trong TrafficRB.ioc với Hình 2 của đề; xác nhận PB3/PB4 được giải phóng nhưng SWD vẫn sử dụng PA13/14. Kiểm tra luồng mode_run → manual_enter khi đang vàng và khi MODE trùng NEXT.

**Một thay đổi có thể tự làm trước khi có board:** bổ sung test hai nút cùng nhấn trong 3 mẫu và một chuỗi dội không đều; ghi trạng thái đèn, số lần chuyển và mode kỳ vọng/thực tế. Khi có board dùng BOARD_BRINGUP=1, điền bảng màu hai module và bitmask nút bằng số đo, cập nhật board_config.h. Tách commit test và commit hiệu chuẩn.

File gợi ý: tests/test_controller.c, TrafficRB/Core/Inc/board_config.h, docs/verification.md. Nhánh gợi ý: feature/manual-button-tests. Kiểm tra email Git trước khi tạo commit đầu.
