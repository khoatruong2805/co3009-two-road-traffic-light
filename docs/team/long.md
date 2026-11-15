# Long · DHT20, LCD và Tuning

Theo dấu một lần đo: gửi AC 33 00, chờ chuyển đổi, đọc 7 byte, kiểm tra busy/calibration/CRC và đổi đơn vị. Phân biệt dữ liệu giả của host với dữ liệu trên dây. Kiểm tra menu Tuning không áp dụng giá trị sai.

**Một thay đổi có thể tự làm:** thêm test cho giới hạn 1/99 giây và dữ liệu DHT20 busy liên tục rồi hồi phục; xác nhận ứng dụng tiếp tục đèn, xóa dữ liệu cũ và hiển thị số liệu mới. Có thể mở rộng mock PCF8574 để kiểm tra vị trí RS/E và nội dung hai dòng, thay vì chỉ kiểm tra đã gửi dữ liệu. Trên board ghi địa chỉ LCD thật, so sánh một lần đọc với điều kiện phòng.

File gợi ý: tests/test_controller.c, TrafficRB/Core/Src/dht20.c, TrafficRB/Core/Src/lcd.c, report/body.tex. Nhánh gợi ý: feature/environment-tuning-tests. Commit test, kết quả và thay đổi đã kiểm tra bằng tài khoản của mình.
