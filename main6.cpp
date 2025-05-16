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

class account; 

enum loaitaikhoan {
    user = 1,
    admin = 0
};

class Wallet {
private:
    string wallet_id; 
    double balance;   
public:
    static vector<string> transaction_log; 
    static Wallet total_wallet; 
    static const string WALLET_FILE; 
    static const string TRANSACTION_FILE; 

    Wallet() : balance(0.0) {
        wallet_id = generateWalletId();
    }

    Wallet(double initial_balance) : balance(initial_balance) {
        wallet_id = generateWalletId();
    }

    string getWalletId() const { return wallet_id; }
    double getBalance() const { return balance; }
    void setBalance(double b) { balance = b; }
// tạo id ví
    static string generateWalletId() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 9);
        string id;
        id.reserve(10);
        for (int i = 0; i < 10; ++i) {
            id += '0' + distrib(gen);
        }
        return id;
    }
//lưu dữ liệu ví
    static bool saveWallets(const vector<account>& accounts) {
        ofstream file(WALLET_FILE);
        if (!file.is_open()) return false;
        for (const auto& acc : accounts) {
            file << acc.getUsername() << "|" << acc.getWallet().getWalletId() << "|" << acc.getWallet().getBalance() << "\n";
        }
        file.close();
        return true;
    }
//tải dữ liệu ví
    static bool loadWallets(vector<account>& accounts) {
        ifstream file(WALLET_FILE);
        if (!file.is_open()) return false;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, wallet_id;
            double balance;
            getline(ss, username, '|');
            getline(ss, wallet_id, '|');
            ss >> balance;
            for (auto& acc : accounts) {
                if (acc.getUsername() == username) {
                    Wallet w;
                    w.wallet_id = wallet_id;
                    w.balance = balance;
                    acc.setWallet(w);
                    break;
                }
            }
        }
        file.close();
        return true;
    }
//lưu lịch sử giao dịch
    static bool saveTransactionLog(const string& transaction) {
        ofstream file(TRANSACTION_FILE, ios::app);
        if (!file.is_open()) return false;
        file << transaction << "\n";
        file.close();
        return true;
    }
//tải lịch sử giao dịch
    static bool loadTransactionLog() {
        ifstream file(TRANSACTION_FILE);
        if (!file.is_open()) return false;
        transaction_log.clear();
        string line;
        while (getline(file, line)) {
            transaction_log.push_back(line);
        }
        file.close();
        return true;
    }
};

vector<string> Wallet::transaction_log;
const string Wallet::WALLET_FILE = "wallets.txt";
const string Wallet::TRANSACTION_FILE = "transactions.txt";
Wallet Wallet::total_wallet(1000000.0); 

class account {
private:
    string hashotp;
    string username;
    string hashpassword;
    string email;
    string phone;
    string id;
    loaitaikhoan loai;
    Wallet wallet;

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

    Wallet getWallet() const { return wallet; }
    void setWallet(const Wallet& w) { wallet = w; }
    string getUsername() const { return username; }

    static void xemThongtin(){

        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);

        auto it = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(), [&user](const account& acc) {return acc.username == user;});

        if(it == danhsachtaikhoan.end()){
            cout << "Tai khoan khong ton tai.\n";
            return;
        }

        account& acc = *it;
        
                string matkhau;
                cout << "Nhap mat khau: ";
                getline(cin, matkhau);
                if(acc.kiemtramatkhau(matkhau)){
                cout << "Thong tin tai khoan:\n";
                cout << "Ten tai khoan: " << acc.username << "\n";
                cout << "Email: " << acc.email << "\n";
                cout << "So dien thoai: " << acc.phone << "\n";
                return;
                }
                else{
                    cout << "Mat khau nhap khong dung.\n";
                    return;
                }
            }
   
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
            if (luuDuLieu() && Wallet::saveWallets(danhsachtaikhoan)) {
                cout << "Thong tin da duoc cap nhat.\n";
                return;
            }
            else {
                cout << "Loi khi luu du lieu.\n";
                return;
            }
    }

    static void chuyenDiem() {
        string userA, walletIdB;
        double points;
        cout << "Nhap ten tai khoan nguon: ";
        getline(cin, userA);
        cout << "Nhap ma vi dich: ";
        getline(cin, walletIdB);
        cout << "Nhap so diem can chuyen: ";
        cin >> points;
        cin.ignore();

        auto itA = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(), [&userA](const account& acc) { return acc.username == userA; });
        if (itA == danhsachtaikhoan.end()) {
            cout << "Tai khoan nguon khong ton tai.\n";
            return;
        }

        auto itB = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(), [&walletIdB](const account& acc) { return acc.wallet.getWalletId() == walletIdB; });
        if (itB == danhsachtaikhoan.end() && walletIdB != Wallet::total_wallet.getWalletId()) {
            cout << "Vi dich khong ton tai.\n";
            return;
        }
         account& accA = *itA;
        Wallet& walletB = (walletIdB == Wallet::total_wallet.getWalletId()) ? Wallet::total_wallet : itB->wallet;

        string password;
        cout << "Nhap mat khau tai khoan nguon: ";
        getline(cin, password);
        if (!accA.kiemtramatkhau(password)) {
            cout << "Mat khau khong dung.\n";
            return;
        }

        string otp = taoOTP();
        accA.hashotp = accA.hashOTP(otp);
        cout << "Vui long nhap ma OTP ben duoi de xac nhan giao dich.\n";
        cout << otp << "\n";
        string nhapOTP;
        cout << "Nhap OTP: ";
        getline(cin, nhapOTP);
        if (!accA.kiemtraOTP(nhapOTP)) {
            cout << "Xac nhan OTP that bai.\n";
            return;
        }

        // Giao dịch atomic
        if (accA.wallet.getBalance() >= points) {
            accA.wallet.setBalance(accA.wallet.getBalance() - points);
            walletB.setBalance(walletB.getBalance() + points);

            string transaction = "Chuyen " + to_string(points) + " diem tu vi " + accA.wallet.getWalletId() + " den vi " + walletIdB + " (Thanh cong)";
            if (Wallet::saveWallets(danhsachtaikhoan) && Wallet::saveTransactionLog(transaction)) {
                cout << "Giao dich thanh cong.\n";
            } else {
                accA.wallet.setBalance(accA.wallet.getBalance() + points);
                walletB.setBalance(walletB.getBalance() - points);
                cout << "Loi khi luu giao dich.\n";
            }
        } else {
            string transaction = "Chuyen " + to_string(points) + " diem tu vi " + accA.wallet.getWalletId() + " den vi " + walletIdB + " (That bai: So du khong du)";
            Wallet::saveTransactionLog(transaction);
            cout << "So du khong du. Khong the thuc hien giao dich.\n";
        }
    }

    static void xemLichSuGiaoDich() {
        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);

        auto it = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(), [&user](const account& acc) { return acc.username == user; });
        if (it == danhsachtaikhoan.end()) {
            cout << "Tai khoan khong ton tai.\n";
            return;
        }

        account& acc = *it;
        string matkhau;
        cout << "Nhap mat khau: ";
        getline(cin, matkhau);
        if (!acc.kiemtramatkhau(matkhau)) {
            cout << "Mat khau khong dung.\n";
            return;
        }

        cout << "Lich su giao dich cua vi " << acc.wallet.getWalletId() << ":\n";
        for (const auto& transaction : Wallet::transaction_log) {
            if (transaction.find(acc.wallet.getWalletId()) != string::npos) {
                cout << transaction << "\n";
            }
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

     // Khởi tạo ví với số dư ban đầu từ ví tổng
        double initial_points = 1000.0; 
        if (Wallet::total_wallet.getBalance() >= initial_points) {
            wallet.setBalance(initial_points);
            Wallet::total_wallet.setBalance(Wallet::total_wallet.getBalance() - initial_points);
            string transaction = "Khoi tao vi " + wallet.getWalletId() + " voi " + to_string(initial_points) + " diem tu vi tong";
            Wallet::saveTransactionLog(transaction);
        } else {
            cout << "Vi tong khong du diem de cap phat.\n";
            return;
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
        cout << "\n1. Dang ky\n2. Admin tao tai khoan\n3. Dang nhap\n4. Quen mat khau\n5. Doi mat khau\n6. Thay doi thong tin\n7. Xem thong tin\n0. Thoat\nChon: ";
        cin >> choices;
        cin.ignore();

        account acc;
        if (choices == 1) acc.dangky(false);
        else if (choices == 2) acc.dangky(true);
        else if (choices == 3) acc.dangnhap();
        else if (choices == 4) acc.quenMatKhau();
        else if (choices == 5) acc.doiMatKhau();
        else if (choices == 6) acc.thaydoiThongtin();
        else if (choices == 7) acc.xemThongtin();
        else if (choices != 0) cout << "lua chon khong hop le\n";
    } while (choices != 0);
    cout << "thoat chuong trinh";
    return 0;
}
