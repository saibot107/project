
#include <iostream>
#include "bcrypt.h"
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
using namespace std;

enum loaitaikhoan {
    user = 1,
    admin = 0
};

class account {
private:
    string username;
    string hashpassword;
    string email;
    string phone;
    string id;
    loaitaikhoan loai;

    string hashPassword(const string& password) {
        return bcrypt::generateHash(password);
    }

    bool validateEmail(const string& email) {
        const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return regex_match(email, pattern);
    }

    bool validatePhone(const string& phone) {
        const regex pattern(R"(0[0-9]{9})");
        return regex_match(phone, pattern);
    }

public:
    static vector<account> danhsachtaikhoan;
    static const string DATA_FILE;

    static void quenMatKhau() {
        string user;
        cout << "Nhap ten tai khoan:";
        getline(cin, user);

        int method;
        cout << "Chon phuong thuc xac minh: \n1. Email \n2. So dien thoai \nLua chon:";
        cin >> method;
        cin.ignore();

        for (auto& acc : danhsachtaikhoan) {
            if (acc.username == user) {
                bool xacThuc = false;

                if (method == 1) {
                    string inputEmail;
                    cout << "Nhap Email: ";
                    getline(cin, inputEmail);
                    if (acc.email == inputEmail) xacThuc = true;
                }
                else if (method == 2) {
                    string inputPhone;
                    cout << "Nhap so dien thoai: ";
                    getline(cin, inputPhone);
                    if (acc.phone == inputPhone) xacThuc = true;
                }
                else {
                    cout << "Lua chon khong hop le \n";
                    return;
                }

                if (xacThuc) {
                    cout << "Xac thuc thanh cong.\nNhap mat khau moi: ";
                    string newPassword;
                    getline(cin, newPassword);
                    acc.hashpassword = bcrypt::generateHash(newPassword);

                    if (luuDuLieu()) {
                        cout << "Mat khau da duoc cap nhat.\n";
                    }
                    else {
                        cout << "Loi khi luu du lieu.\n";
                    }
                }
                else {
                    cout << "Xac thuc that bai.\n";
                }
                return;
            }
        }
        cout << "Khong tim thay tai khoan.\n";
    }

    void clrscr() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    bool kiemtramatkhau(const string& password) {
        return bcrypt::validatePassword(password, hashpassword);
    }

    static bool kiemtrataikhoantrung(const string& user) {
        for (const auto& acc : danhsachtaikhoan) {
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
            getline(ss, acc.username, '|');
            getline(ss, acc.hashpassword, '|');
            getline(ss, acc.email, '|');
            getline(ss, acc.phone, '|');
            getline(ss, loaiStr);

            try {
                acc.loai = static_cast<loaitaikhoan>(stoi(loaiStr));
                danhsachtaikhoan.push_back(acc);
            } catch (...) {
                continue;
            }
        }
        file.close();
        return true;
    }
};

vector<account> account::danhsachtaikhoan;
const string account::DATA_FILE = "accounts.txt";

int main() {
    account::taiDuLieu();
    int choices;
    if (!account::taiDuLieu()) {
        cout << "Khong the tai du lieu, tao file moi\n";
    }
    do {
        cout << "\n1. Dang ky\n2. Admin tao tai khoan\n3. Dang nhap\n4. Quen mat khau\n0. Thoat\nChon: ";
        cin >> choices;
        cin.ignore();

        account acc;
        if (choices == 1) acc.dangky(false);
        else if (choices == 2) acc.dangky(true);
        else if (choices == 3) acc.dangnhap();
        else if (choices == 4) acc.quenMatKhau();
        else if (choices != 0) cout << "lua chon khong hop le\n";
    } while (choices != 0);
    cout << "thoat chuong trinh";
    return 0;
}
