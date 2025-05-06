if (acc.username == user) return true;
        }
        return false;
    }

    void dangky(bool laAdmin = false) {
        cout << "nhap tai khoan: ";
        getline(cin, username);

        if (kiemtrataikhoantrung(username)) {
            cout << "tai khoan da ton tai";
            return;
        }

        cout << "nhap mat khau: ";
        string password;
        getline(cin, password);
        hashpassword = hashPassword(password);

        cout << "nhap email: ";
        getline(cin, email);

        cout << "nhap sdt: ";
        getline(cin, phone);

        if (laAdmin) {
            loai = user;
        }
        else {
            int loai_;
            cout << "chon loai tai khoan: \n0. quan ly \n1. nguoi dung \nchon: ";
            cin >> loai_;
            cin.ignore();
            loai = (loai_ == 0) ? admin : user;
        }

        danhsachtaikhoan.push_back(*this);
        cout << "dang ky thanh cong, enter de tiep tuc";
        cin.get();
        clrscr();
    }

    static void dangnhap() {
        string inputpassword, user;
        cout << "nhap tai khoan de dang nhap: ";
        getline(cin, user);
        cout << "nhap mat khau de dang nhap: ";
        getline(cin, inputpassword);

        for (auto& acc : danhsachtaikhoan) {
            if (acc.username == user && acc.kiemtramatkhau(inputpassword)) {
                cout << "dang nhap thanh cong";
                return;
            }
        }
        cout << "sai tai khoan hoac mat khau";
    }

    static void doiMatKhau() {
        string user, oldPassword;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);
        cout << "Nhap mat khau cu: ";
        getline(cin, oldPassword);

        for (auto& acc : danhsachtaikhoan) {
            if (acc.username == user && acc.kiemtramatkhau(oldPassword)) {
                string newPassword;
                cout << "Nhap mat khau moi: ";
                getline(cin, newPassword);
                acc.hashpassword = bcrypt::generateHash(newPassword);

                if (luuDuLieu()) {
                    cout << "Doi mat khau thanh cong.\n";
                } else {
                    cout << "Loi khi luu du lieu.\n";
                }
                return;
            }
        }
        cout << "Ten tai khoan hoac mat khau cu sai.\n";
    }

    static bool luuDuLieu() {
        ofstream file(DATA_FILE);
        if (!file.is_open()) return false;

        for (const auto& acc : danhsachtaikhoan) {
            file << acc.username << "|" << acc.hashpassword << "|" << acc.email << "|" << acc.phone << "|" << acc.loai << "\n";
        }
        file.close();
        return true;
    }

    static bool taiDuLieu() {
        ifstream file(DATA_FILE);
        if (!file.is_open()) return false;

        danhsachtaikhoan.clear();
        string line;
        while (getline(file, line)) {
            account acc;
            stringstream ss(line);
            string loaiStr;
