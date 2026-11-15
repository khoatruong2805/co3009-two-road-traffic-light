# Mỗi thành viên tự commit như thế nào?

Một repo cho cả nhóm. Commit khởi tạo là mốc chung; việc có tên trong bảng phân công chưa phải bằng chứng đóng góp. Mỗi người clone và kiểm tra bằng tài khoản riêng, chọn một việc thật ở phiếu dưới đây, tự sửa code/test hoặc ghi phép đo do mình thực hiện rồi mở PR vào `dev`. Không dùng chung tài khoản và không đổi tác giả commit thay nhau.

- [Khoa – scheduler, timer, Auto](docs/team/khoa.md)
- [Minh – board, button, Manual, chuyển mode](docs/team/minh.md)
- [Long – DHT20, LCD, Tuning](docs/team/long.md)

Khoa thêm đúng username vào Settings → Collaborators, quyền Write; thành viên phải nhận lời mời. Nếu chưa được mời, có thể fork, tạo nhánh trên fork, mở PR về `dev` của repo nhóm. GitHub chỉ gắn commit với tài khoản nếu email author đã được xác minh hoặc trùng địa chỉ noreply do GitHub cấp. Kiểm tra **Settings → Emails** của mình; nhánh Minh bên repo tham khảo có ví dụ commit hiện tên tác giả nhưng chưa liên kết tài khoản.

Ví dụ từ máy của một thành viên (đổi tên nhánh theo việc thật):

```bash
git clone https://github.com/khoatruong2805/co3009-two-road-traffic-light.git
cd co3009-two-road-traffic-light
git switch dev
git pull --ff-only
git switch -c feature/scheduler-boundaries
git config --local user.name "Tên của mình"
git config --local user.email "địa chỉ đã xác minh trên GitHub của mình"
# Làm việc, chạy test, đọc diff, chỉ stage các file mình đã sửa.
git add -p
# Với file mới: git add đường/dẫn/tên-file
git diff --cached
git commit -m "scheduler: test cancellation at equal deadlines"
git push -u origin HEAD
```

Trong PR ghi: thay đổi gì, vì sao, chạy thử thế nào, phần nào chưa kiểm tra. Người khác review rồi **Create a merge commit** để giữ nguyên từng commit. Khoa đối chiếu `git shortlog -sne dev`, mở commit trên GitHub xem có liên kết đúng avatar/tài khoản. Khi nhóm đã demo và hoàn thiện: merge dev → main, tạo tag `v1.0-final`, ghi `git rev-parse HEAD` và tag trên bìa, nộp đúng cổng môn. Không tag bản nền là bản cuối.
