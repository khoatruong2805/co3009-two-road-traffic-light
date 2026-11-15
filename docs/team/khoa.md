# Khoa · Scheduler và Auto

Đọc các cặp scheduler, software_timer, fsm_automatic trong TrafficRB/Core. Xem vì sao ISR chỉ tăng tick và kiểm tra head; chèn/xóa có thể O(n) ở foreground. Kiểm tra quyết định bỏ qua lần chạy đã trễ của task định kỳ.

**Một thay đổi có thể tự làm:** thêm test callback tự xóa task định kỳ của chính nó; thêm hai task chung deadline, xóa head trước khi tick, kiểm tra callback còn lại không bị muộn. Trước khi sửa, dự đoán dãy tick/callback; chạy và ghi kết quả thực. Nếu thấy lỗi, sửa và ghi case hồi quy. Sau đó cập nhật mục Scheduler và Automatic của report theo kết quả của mình.

File gợi ý: tests/test_controller.c, TrafficRB/Core/Src/scheduler.c, report/body.tex. Nhánh gợi ý: feature/scheduler-boundaries. Chỉ commit phần mình đã làm và kiểm tra.
