#include <iostream>
#include "bcrypt.h"
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

enum loaitaikhoan {
    user = 1,
    admin = 0
};

class account {
private:
    string hashotp;
    string username;
    string hashpassword;
    string email;
    string phone;
    string id;
    loaitaikhoan loai;

    string hashOTP(const string& otp){
        return bcrypt::generateHash(otp);
    }
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

    static string taoOTP(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 9);

        string otp;
        otp.reserve(6);

        for(int i = 0; i < 6; ++i){
            otp += '0' + distrib(gen);
        }
        return otp;
    }

public:
    static vector<account> danhsachtaikhoan;
    static const string DATA_FILE;

    static void thaydoiThongtin(){

        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);

        auto it = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(), [&user](const account& acc) {return acc.username == user;});
        
        if(it == danhsachtaikhoan.end()){
            cout << "Tai khoan khong ton tai.\n";
            return;
        }

        account& acc = *it;

        int Luachon;
        cout << "Chon thong tin muon thay doi: \n1. Email\n2. So dien thoai\n3. Ten tai khoan.\nLua chon: ";
        cin >> Luachon;
        cin.ignore();

        string otp = taoOTP();
        acc.hashotp = acc.hashOTP(otp);

                bool xacThuc = false;

                switch(Luachon){
                    case 1:{                     //Email
                        string nhapEmail;
                        cout << "Nhap Email hien tai: ";
                        getline(cin, nhapEmail);
                        if(acc.email == nhapEmail){
                            string emailNew;
                            cout << "Nhap Email moi: ";
                            getline(cin, emailNew);
                            if(acc.validateEmail(emailNew)){
                            acc.email = emailNew;
                            cout << "Email " << nhapEmail << " cua ban se thay doi thanh " << emailNew << ".\n";
                            cout << "Vui long nhap ma OTP ben duoi de xac nhan.\n";
                            cout << otp << "\n";
                            string nhapOTP;
                            cout << "Nhap OTP: ";
                            getline(cin, nhapOTP);
                            if(acc.kiemtraOTP(nhapOTP)){
                                cout << "Xac nhan thanh cong.\n";
                            }
                            else{
                                cout << "Xac nhan that bai.\n";
                                return;
                            }
                        }else{
                            cout << "Email da ton tai.\n";
                            return;
                        }
                    }   
                    else{
                        cout << "Email khong dung.\n";
                        return;
                    }   
                        break;
                    }
                    case 2:{                        //Phone
                        string nhapPhone;
                        cout << "Nhap so dien thoai hien tai: ";
                        getline(cin, nhapPhone);
                        if(acc.phone == nhapPhone){
                            string phoneNew;
                            cout << "Nhap so dien thoai moi: ";
                                getline(cin, phoneNew);
                                if(acc.validatePhone(phoneNew)){
                                acc.phone = phoneNew;
                                cout << "So dien thoai " << nhapPhone << " cua ban se thay doi thanh " << phoneNew << ".\n";
                                cout << "Vui long nhap ma OTP ben duoi de xac nhan.\n";
                                cout << otp << "\n";
                                string nhapOTP;
                                cout << "Nhap OTP: ";
                                getline(cin, nhapOTP);
                                if(acc.kiemtraOTP(nhapOTP)){
                                    cout << "Xac nhan thanh cong.\n";
                                }else{
                                    cout << "Xac nhan that bai.\n";
                                    return;
                                }
                                }else {
                                cout << "So dien thoai da ton tai.\n";
                                return;
                            }
                        }else {
                            cout << "So dien thoai khong dung.\n";
                            return;
                        }
                        break;
                    }
                    case 3:{                         //TenTaiKhoan
                        string nhapTen;
                        cout << "Nhap mat khau: ";
                        getline(cin, nhapTen);
                        if(acc.kiemtramatkhau(nhapTen)){
                            string tenNew;
                            cout << "Nhap ten tai khoan moi: ";
                            getline(cin, tenNew);
                            if(!kiemtrataikhoantrung(tenNew)){
                                acc.username = tenNew;
                                cout << "Ten tai khoan " << user << " cua ban se thay doi thanh " << tenNew << ".\n";
                                cout << "Vui long nhap ma OTP ben duoi de xac nhan.\n";
                                cout << otp << "\n";
                                string nhapOTP;
                                cout << "Nhap OTP: ";
                                getline(cin, nhapOTP);
                                if(acc.kiemtraOTP(nhapOTP)){
                                    cout << "Xac nhan thanh cong.\n";
                                }
                                else{
                                    cout << "Xac nhan that bai.\n";
                                    return;
                                }
                            }
                            else {
                                cout << "Ten tai khoan da ton tai.\n";
                                return;
                            }
                        }
                        else{
                            cout << "Mat khau khong dung.\n";
                            return;
                        }
                        break;
                    }
                    default:{
                        cout << "Lua chon khong hop le.\n";
                        return;
                    }
            }
             if (luuDuLieu()) {
                cout << "Thong tin da duoc cap nhat.\n";
                return;
            }
            else {
                cout << "Loi khi luu du lieu.\n";
                return;
            }
    }

    static void quenMatKhau() {
        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);

        int method;
        cout << "Chon phuong thuc xac minh: \n1. Email \n2. So dien thoai \nLua chon: ";
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

    bool kiemtraOTP(const string& otp){
        return bcrypt::validatePassword(otp, hashotp);
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
        cout << "\n1. Dang ky\n2. Admin tao tai khoan\n3. Dang nhap\n4. Quen mat khau\n5. Doi mat khau\n6. Thay doi thong tin\n0. Thoat\nChon: ";
        cin >> choices;
        cin.ignore();

        account acc;
        if (choices == 1) acc.dangky(false);
        else if (choices == 2) acc.dangky(true);
        else if (choices == 3) acc.dangnhap();
        else if (choices == 4) acc.quenMatKhau();
        else if (choices == 5) acc.doiMatKhau();
        else if (choices == 6) acc.thaydoiThongtin();
        else if (choices != 0) cout << "lua chon khong hop le\n";
    } while (choices != 0);
    cout << "thoat chuong trinh";
    return 0;
}
