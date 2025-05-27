- Các thành viên kèm với công việc được giao

phần A:
+ tạo mới tài khoản khi đăng ký : Lê Phạm Khánh Huy
+ lưu trữ dữ liệu : Lê Phạm Khánh Huy, Phan Giảng Bình
+ sinh mật khẩu tự động : 
+ thay đổi mật khẩu : Trần Quang Huy
+ đăng nhập : Lê Phạm Khánh Huy

phần B:
+ các chức năng của người dùng : Lê Ngọc Dũng
+ các chức năng của quản lý : Lê Phạm Khánh Huy

phần C: 
+ giao dịch chuyển điểm, lịch sử giao dịch : Trần Quang Huy
+ những phần còn lại : Phan Giảng Bình

- đặc tả các chức năng : 
Hệ thống quản lý tài khoản

1. Menu chính 
Mô tả: Hiển thị khi chương trình khởi động và người dùng chưa đăng nhập. Đây là điểm vào chính của hệ thống.
Các tùy chọn: 
+ Đăng ký: Cho phép tạo tài khoản mới với thông tin như tên tài khoản, mật khẩu (mã hóa bằng bcrypt), email, số điện thoại, và loại tài khoản (admin/người dùng). Tài khoản mới được cấp ví với 1000 điểm từ ví tổng.
+ Đăng nhập: Yêu cầu nhập tên tài khoản và mật khẩu. Nếu thành công, chuyển đến menu chức năng (người dùng) hoặc menu admin (admin).
+ Quên mật khẩu: Cho phép khôi phục mật khẩu bằng cách xác minh qua email hoặc số điện thoại, sau đó nhập mật khẩu mới.
+ Thoát: Kết thúc chương trình.

2. Menu chức năng 
Mô tả: Dành cho người dùng đã đăng nhập (không phải admin). Cung cấp các chức năng quản lý tài khoản và ví cá nhân.
Các tùy chọn: 
+ Đổi mật khẩu: Yêu cầu nhập mật khẩu cũ và mật khẩu mới để cập nhật.
+ Thay đổi thông tin: Cho phép cập nhật email, số điện thoại, hoặc tên tài khoản sau khi xác minh thông tin hiện tại và OTP.
+ Xem thông tin: Hiển thị thông tin tài khoản (tên, email, số điện thoại, số dư ví, loại tài khoản) sau khi xác minh mật khẩu.
+ Chuyển điểm: Cho phép chuyển điểm từ ví của người dùng sang ví khác (bao gồm ví tổng) sau khi xác minh mật khẩu và OTP.
+ Xem lịch sử giao dịch: Hiển thị các giao dịch liên quan đến ví của người dùng.
+ Đăng xuất: Thoát trạng thái đăng nhập, quay lại menu chính.

3. Menu quản trị viên 
Mô tả: Dành riêng cho tài khoản admin, cung cấp các chức năng quản lý hệ thống.
Các tùy chọn: 
+ Theo dõi danh sách tài khoản: Hiển thị danh sách tài khoản, phân loại thành hai nhóm: tài khoản có giao dịch và tài khoản chưa có giao dịch. Hiển thị thông tin như tên, email, số điện thoại, số dư ví, và loại tài khoản.
+ Điều chỉnh thông tin tài khoản: Cho phép admin thay đổi email, số điện thoại, hoặc tên tài khoản của bất kỳ người dùng nào sau khi xác minh danh tính admin và OTP từ người dùng.
+ Tạo tài khoản mới: Tạo tài khoản mới với quyền admin hoặc người dùng.
+ Đăng xuất: Thoát trạng thái đăng nhập, quay lại menu chính.
