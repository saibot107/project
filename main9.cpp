#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include "bcrypt.h"
using namespace std;
void clrscr(){
    system("cls");
}
enum loaitaikhoan {
    admin = 0,
    user = 1
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
    static bool saveTransactionLog(const string& transaction) {
        ofstream file(TRANSACTION_FILE, ios::app);
        if (!file.is_open()) return false;
        file << transaction << "\n";
        file.close();
        return true;
    }
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
    string hashOTP(const string& otp) {
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
    static string taoOTP() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 9);
        string otp;
        otp.reserve(6);
        for (int i = 0; i < 6; ++i) {
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
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    loaitaikhoan getLoai() const { return loai; }
    void setEmail(const string& e) { email = e; }
    void setPhone(const string& p) { phone = p; }
    void setUsername(const string& u) { username = u; }
    static bool saveWallets(const vector<account>& accounts) {
        ofstream file(Wallet::WALLET_FILE);
        if (!file.is_open()) return false;
        for (const auto& acc : accounts) {
            file << acc.getUsername() << "|" << acc.getWallet().getWalletId() << "|" << acc.getWallet().getBalance() << "\n";
        }
        file.close();
        return true;
    }
    static bool loadWallets(vector<account>& accounts) {
        ifstream file(Wallet::WALLET_FILE);
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
                    w.setBalance(balance);
                    acc.setWallet(w);
                    break;
                }
            }
        }
        file.close();
        return true;
    }

    static string matkhautusinh(){
        const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, chars.size() - 1);

        string matkhau;
        matkhau.reserve(8);
        for( int i = 0; i < 8; i++){
            matkhau += chars[distrib(gen)];
        }
        return matkhau;
    }
    static void xemThongtin() {
        clrscr();
        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);
        auto it = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),[&user](const account& acc) { return acc.username == user; });
        if (it == danhsachtaikhoan.end()) {
            cout << "Tai khoan khong ton tai.\n";
            return;
        }
        account& acc = *it;
        string matkhau;
        cout << "Nhap mat khau: ";
        getline(cin, matkhau);
        if (acc.kiemtramatkhau(matkhau)) {
            cout << "Thong tin tai khoan:\n";
            cout << "Ten tai khoan: " << acc.username << "\n";
            cout << "Email: " << acc.email << "\n";
            cout << "So dien thoai: " << acc.phone << "\n";
            cout << "So du vi: " << acc.wallet.getBalance() << "\n";
            cout << "Loai tai khoan: " << (acc.loai == admin ? "Quan ly" : "Nguoi dung") << "\n";
        } else {
            cout << "Mat khau nhap khong dung.\n";
        }
    }
    static void thaydoiThongtin() {
        clrscr();
        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);
        auto it = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),[&user](const account& acc) { return acc.username == user; });
        if (it == danhsachtaikhoan.end()) {
            cout << "Tai khoan khong ton tai.\n";
            return;
        }
        account& acc = *it;
        int Luachon;
        cout << "Chon thong tin muon thay doi: \n1. Email\n2. So dien thoai\n3. Ten tai khoan\nLua chon: ";
        cin >> Luachon;
        cin.ignore();
        string otp = taoOTP();
        acc.hashotp = acc.hashOTP(otp);
        switch(Luachon) {
            case 1: {
                string nhapEmail;
                cout << "Nhap Email hien tai: ";
                getline(cin, nhapEmail);
                if (acc.email == nhapEmail) {
                    string emailNew;
                    cout << "Nhap Email moi: ";
                    getline(cin, emailNew);
                    if (acc.validateEmail(emailNew)) {
                        cout << "Vui long nhap ma OTP ben duoi de xac nhan.\n";
                        cout << otp << "\n";
                        string nhapOTP;
                        cout << "Nhap OTP: ";
                        getline(cin, nhapOTP);
                        if (acc.kiemtraOTP(nhapOTP)) {
                            acc.email = emailNew;
                            cout << "Cap nhat Email thanh cong!\n";
                        } else {
                            cout << "OTP khong dung!\n";
                        }
                    } else {
                        cout << "Email khong hop le!\n";
                    }
                } else {
                    cout << "Email khong dung!\n";
                }
                break;
            }
            case 2: {
                string nhapPhone;
                cout << "Nhap so dien thoai hien tai: ";
                getline(cin, nhapPhone);
                if (acc.phone == nhapPhone) {
                    string phoneNew;
                    cout << "Nhap so dien thoai moi: ";
                    getline(cin, phoneNew);
                    if (acc.validatePhone(phoneNew)) {
                        cout << "Vui long nhap ma OTP ben duoi de xac nhan.\n";
                        cout << otp << "\n";
                        string nhapOTP;
                        cout << "Nhap OTP: ";
                        getline(cin, nhapOTP);
                        if (acc.kiemtraOTP(nhapOTP)) {
                            acc.phone = phoneNew;
                            cout << "Cap nhat so dien thoai thanh cong!\n";
                        } else {
                            cout << "OTP khong dung!\n";
                        }
                    } else {
                        cout << "So dien thoai khong hop le!\n";
                    }
                } else {
                    cout << "So dien thoai khong dung!\n";
                }
                break;
            }
            case 3: {
                string nhapMatKhau;
                cout << "Nhap mat khau: ";
                getline(cin, nhapMatKhau);
                if (acc.kiemtramatkhau(nhapMatKhau)) {
                    string tenNew;
                    cout << "Nhap ten tai khoan moi: ";
                    getline(cin, tenNew);
                    if (!kiemtrataikhoantrung(tenNew)) {
                        cout << "Vui long nhap ma OTP ben duoi de xac nhan.\n";
                        cout << otp << "\n";
                        string nhapOTP;
                        cout << "Nhap OTP: ";
                        getline(cin, nhapOTP);
                        if (acc.kiemtraOTP(nhapOTP)) {
                            acc.username = tenNew;
                            cout << "Cap nhat ten tai khoan thanh cong!\n";
                        } else {
                            cout << "OTP khong dung!\n";
                        }
                    } else {
                        cout << "Ten tai khoan da ton tai!\n";
                    }
                } else {
                    cout << "Mat khau khong dung!\n";
                }
                break;
            }
            default:
                cout << "Lua chon khong hop le!\n";
        }
    }
    static void chuyenDiem() {
        clrscr();
        string userA, walletIdB;
        double points;
        cout << "Nhap ten tai khoan nguon: ";
        getline(cin, userA);
        cout << "Nhap ma vi dich: ";
        getline(cin, walletIdB);
        cout << "Nhap so diem can chuyen: ";
        cin >> points;
        cin.ignore();
        auto itA = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),
                          [&userA](const account& acc) { return acc.username == userA; });
        if (itA == danhsachtaikhoan.end()) {
            cout << "Tai khoan nguon khong ton tai.\n";
            return;
        }
        auto itB = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),
                          [&walletIdB](const account& acc) { return acc.wallet.getWalletId() == walletIdB; });
        if (itB == danhsachtaikhoan.end() && walletIdB != Wallet::total_wallet.getWalletId()) {
            cout << "Vi dich khong ton tai.\n";
            return;
        }
        account& accA = *itA;
        Wallet& walletB = (walletIdB == Wallet::total_wallet.getWalletId()) ?
                         Wallet::total_wallet : itB->wallet;
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
        if (accA.wallet.getBalance() >= points) {
            accA.wallet.setBalance(accA.wallet.getBalance() - points);
            walletB.setBalance(walletB.getBalance() + points);
            string transaction = "Chuyen " + to_string(points) + " diem tu vi " +
                               accA.wallet.getWalletId() + " den vi " + walletIdB + " (Thanh cong)";
            Wallet::saveTransactionLog(transaction);
            cout << "Giao dich thanh cong.\n";
        } else {
            string transaction = "Chuyen " + to_string(points) + " diem tu vi " +
                               accA.wallet.getWalletId() + " den vi " + walletIdB + " (That bai: So du khong du)";
            Wallet::saveTransactionLog(transaction);
            cout << "So du khong du. Khong the thuc hien giao dich.\n";
        }
    }
    static void xemLichSuGiaoDich() {
        clrscr();
        string user;
        cout << "Nhap ten tai khoan: ";
        getline(cin, user);
        auto it = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),
                         [&user](const account& acc) { return acc.username == user; });
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
        bool found = false;
        for (const auto& transaction : Wallet::transaction_log) {
            if (transaction.find(acc.wallet.getWalletId()) != string::npos) {
                cout << transaction << "\n";
                found = true;
            }
        }
        if (!found) {
            cout << "Khong co giao dich nao.\n";
        }
    }
    
    static void doiMatKhau() {
        clrscr();
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
    bool kiemtramatkhau(const string& password) const {
        return bcrypt::validatePassword(password, hashpassword);
    }
    bool kiemtraOTP(const string& otp) const {
        return bcrypt::validatePassword(otp, hashotp);
    }
    static bool kiemtrataikhoantrung(const string& user) {
        for (const auto& acc : danhsachtaikhoan) {
            if (acc.username == user) return true;
        }
        return false;
    }
    void dangky(bool laAdmin = false) {
        clrscr();
        cout << "Nhap tai khoan: ";
        getline(cin, username);
        if (account::kiemtrataikhoantrung(username)) {
            cout << "Tai khoan da ton tai\n";
            cout << "Enter de tiep tuc";
            cin.get();
            return;
        }

        cout << "\nBan co muon dung mat khau tu sinh?\n";
        cout << "Nhap 1 la co.\nNhap 2 la khong.\n";
        cout << "Lua chon cua ban: ";
        int LuaChon;
        cin >> LuaChon;
        cin.ignore();

        if(LuaChon == 1){
            string matkhau = matkhautusinh();
            hashpassword = hashPassword(matkhau);
            cout << "Mat khau cua ban la: " << matkhau << endl;
        }
        else if(LuaChon == 2){
            cout << "Nhap mat khau: ";
            string password;
            getline(cin, password);
            hashpassword = hashPassword(password);
        }
        else{
            cout << "Gia tri khong hop le.";
            return;
        }
        
        cout << "Nhap email: ";
        getline(cin, email);
        while (!validateEmail(email)) {
            cout << "Email khong hop le. Vui long nhap lai: ";
            getline(cin, email);
        }
        cout << "Nhap sdt: ";
        getline(cin, phone);
        while (!validatePhone(phone)) {
            cout << "So dien thoai khong hop le. Vui long nhap lai: ";
            getline(cin, phone);
        }
        if (laAdmin) {
            loai = admin;
        }
        else {
            int loai_;
            cout << "Chon loai tai khoan: \n0. Quan ly \n1. Nguoi dung \nChon: ";
            cin >> loai_;
            cin.ignore();
            loai = (loai_ == 0) ? admin : user;
        }
        double initial_points = 1000.0;
        if (Wallet::total_wallet.getBalance() >= initial_points) {
            wallet.setBalance(initial_points);
            Wallet::total_wallet.setBalance(Wallet::total_wallet.getBalance() - initial_points);
            string transaction = "Khoi tao vi " + wallet.getWalletId() + " voi " + to_string(initial_points) + " diem tu vi tong";
            Wallet::saveTransactionLog(transaction);
        } else {
            cout << "Vi tong khong du diem de cap phat.\n";
            cout << "Enter de tiep tuc";
            cin.get();
            return;
        }
        danhsachtaikhoan.push_back(*this);
        cout << "Dang ky thanh cong, enter de tiep tuc";
        cin.get();
        clrscr();
    }
    static void dangnhap(account& loggedInAcc, bool& isLoggedIn) {
        clrscr();
        string inputpassword, user;
        cout << "Nhap tai khoan de dang nhap: ";
        getline(cin, user);
        cout << "Nhap mat khau de dang nhap: ";
        getline(cin, inputpassword);
        for (auto& acc : danhsachtaikhoan) {
            if (acc.username == user && acc.kiemtramatkhau(inputpassword)) {
                loggedInAcc = acc;
                isLoggedIn = true;
                cout << "Dang nhap thanh cong!\n";
                cout << "Xin chao " << user << "!\n";
                cout << "So du vi: " << acc.wallet.getBalance() << "\n";
                cout << "Enter de tiep tuc";
                cin.get();
                clrscr();
                return;
            }
        }
        cout << "Sai tai khoan hoac mat khau!\n";
        cout << "Enter de tiep tuc";
        cin.get();
        clrscr();
    }
    static void adminMenu(account& adminAcc) {
        int choice;
        do {
            clrscr();
            cout << "\n=== MENU QUAN LY ===" << endl;
            cout << "1. Theo doi danh sach nhom" << endl;
            cout << "2. Dieu chinh thong tin tai khoan" << endl;
            cout << "3. Tao tai khoan moi" << endl;
            cout << "0. Dang xuat" << endl;
            cout << "Chon: ";
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1:
                    clrscr();
                    adminTheodoiDanhsach();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 2:
                    clrscr();
                    adminDieuChinhThongtin();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 3:
                    {
                        clrscr();
                        account acc;
                        acc.dangky(true);
                        if (luuDuLieu()) {
                            cout << "Tao tai khoan thanh cong!\n";
                        }
                        cout << "Enter de tiep tuc";
                        cin.get();
                    }
                    break;
                case 0:
                    clrscr();
                    cout << "Dang xuat thanh cong.\n";
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
                    cout << "Enter de tiep tuc";
                    cin.get();
            }
        } while (choice != 0);
    }
    static void adminTheodoiDanhsach() {
        vector<account> coGiaoDich;
        vector<account> chuaGiaoDich;
        for (const auto& acc : danhsachtaikhoan) {
            bool hasTransaction = false;
            for (const auto& transaction : Wallet::transaction_log) {
                if (transaction.find(acc.getWallet().getWalletId()) != string::npos) {
                    hasTransaction = true;
                    break;
                }
            }
            if (hasTransaction) {
                coGiaoDich.push_back(acc);
            } else {
                chuaGiaoDich.push_back(acc);
            }
        }
        cout << "\n=== DANH SACH TAI KHOAN CO GIAO DICH ===\n";
        for (const auto& acc : coGiaoDich) {
            cout << "Tai khoan: " << acc.getUsername()
                 << " | Email: " << acc.getEmail()
                 << " | SDT: " << acc.getPhone()
                 << " | So du: " << acc.getWallet().getBalance()
                 << " | Loai: " << (acc.getLoai() == admin ? "Quan ly" : "Nguoi dung") << endl;
        }
        cout << "\n=== DANH SACH TAI KHOAN CHUA CO GIAO DICH ===\n";
        for (const auto& acc : chuaGiaoDich) {
            cout << "Tai khoan: " << acc.getUsername()
                 << " | Email: " << acc.getEmail()
                 << " | SDT: " << acc.getPhone()
                 << " | So du: " << acc.getWallet().getBalance()
                 << " | Loai: " << (acc.getLoai() == admin ? "Quan ly" : "Nguoi dung") << endl;
        }
    }
    static void adminDieuChinhThongtin() {
        clrscr();
        string adminUser, adminPass;
        cout << "Nhap tai khoan admin: ";
        getline(cin, adminUser);
        cout << "Nhap mat khau admin: ";
        getline(cin, adminPass);
        auto adminIt = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),[&adminUser, &adminPass](const account& acc){
            return acc.getUsername() == adminUser && acc.kiemtramatkhau(adminPass) && acc.getLoai() == admin;
        });
        if (adminIt == danhsachtaikhoan.end()) {
            cout << "Xac thuc admin that bai!\n";
            return;
        }
        string targetUser;
        cout << "Nhap tai khoan can dieu chinh: ";
        getline(cin, targetUser);
        auto targetIt = find_if(danhsachtaikhoan.begin(), danhsachtaikhoan.end(),[&targetUser](const account& acc) {
            return acc.getUsername() == targetUser;
        });
        if (targetIt == danhsachtaikhoan.end()) {
            cout << "Tai khoan khong ton tai!\n";
            return;
        }
        account& targetAcc = *targetIt;
        int choice;
        cout << "Chon thong tin can dieu chinh:\n";
        cout << "1. Email\n2. So dien thoai\nChon: ";
        cin >> choice;
        cin.ignore();
        string oldValue, newValue;
        string fieldName;
        switch (choice) {
            case 1:
                cout << "Nhap email moi: ";
                getline(cin, newValue);
                if (!targetAcc.validateEmail(newValue)) {
                    cout << "Email khong hop le!\n";
                    return;
                }
                oldValue = targetAcc.getEmail();
                fieldName = "email";
                targetAcc.setEmail(newValue);
                break;
            case 2:
                cout << "Nhap so dien thoai moi: ";
                getline(cin, newValue);
                if (!targetAcc.validatePhone(newValue)) {
                    cout << "So dien thoai khong hop le!\n";
                    return;
                }
                oldValue = targetAcc.getPhone();
                fieldName = "so dien thoai";
                targetAcc.setPhone(newValue);
                break;
            default:
                cout << "Lua chon khong hop le!\n";
                return;
        }
        string otp = taoOTP();
        targetAcc.hashotp = targetAcc.hashOTP(otp);
        cout << "\n=== THONG BAO THAY DOI THONG TIN ===\n";
        cout << "Tai khoan cua ban da duoc yeu cau thay doi boi quan tri vien\n";
        cout << "Thong tin thay doi: " << fieldName << "\n";
        cout << "Gia tri cu: " << oldValue << "\n";
        cout << "Gia tri moi: " << newValue << "\n";
        cout << "Ma OTP xac nhan: " << otp << "\n\n";
        cout << "Nhap OTP de xac nhan thay doi: ";
        string inputOTP;
        getline(cin, inputOTP);
        if (targetAcc.kiemtraOTP(inputOTP)) {
            if (luuDuLieu()) {
                cout << "Cap nhat thong tin thanh cong!\n";
            } else {
                cout << "Loi khi luu du lieu!\n";
                switch (choice) {
                    case 1: targetAcc.setEmail(oldValue); break;
                    case 2: targetAcc.setPhone(oldValue); break;
                }
            }
        } else {
            cout << "OTP khong dung! Thay doi khong duoc chap nhan.\n";
            switch (choice) {
                case 1: targetAcc.setEmail(oldValue); break;
                case 2: targetAcc.setPhone(oldValue); break;
            }
        }
    }
    static bool luuDuLieu() {
        ofstream file(DATA_FILE);
        if (!file.is_open()) return false;
        for (const auto& acc : danhsachtaikhoan) {
            file << acc.username << "|" << acc.hashpassword << "|" << acc.email << "|" << acc.phone << "|" << acc.loai << "\n";
        }
        file.close();
        if (!saveWallets(danhsachtaikhoan)) return false;
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
        if (!loadWallets(danhsachtaikhoan)) return false;
        if (!Wallet::loadTransactionLog()) return false;
        return true;
    }
    static void functionMenu(account& loggedInAcc, bool& isLoggedIn) {
        int choice;
        do {
            clrscr();
            cout << "\n=== MENU CHUC NANG ===" << endl;
            cout << "1. Doi mat khau" << endl;
            cout << "2. Thay doi thong tin" << endl;
            cout << "3. Xem thong tin" << endl;
            cout << "4. Chuyen diem" << endl;
            cout << "5. Xem lich su giao dich" << endl;
            cout << "0. Dang xuat" << endl;
            cout << "Chon: ";
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1:
                    clrscr();
                    doiMatKhau();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 2:
                    clrscr();
                    thaydoiThongtin();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 3:
                    clrscr();
                    xemThongtin();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 4:
                    clrscr();
                    chuyenDiem();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 5:
                    clrscr();
                    xemLichSuGiaoDich();
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 0:
                    clrscr();
                    cout << "Dang xuat thanh cong.\n";
                    isLoggedIn = false;
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                default:
                    cout << "Lua chon khong hop le!\n";
                    cout << "Enter de tiep tuc";
                    cin.get();
            }
            luuDuLieu();
        } while (choice != 0 && isLoggedIn);
    }
};
vector<account> account::danhsachtaikhoan;
const string account::DATA_FILE = "accounts.txt";
int main() {
    account loggedInAcc;
    bool isLoggedIn = false;
    while (true) {
        if (!isLoggedIn) {
            account tempAcc;
            clrscr();
            int choice;
            cout << "\n=== HE THONG QUAN LY TAI KHOAN ===" << endl;
            cout << "1. Dang ky" << endl;
            cout << "2. Dang nhap" << endl;
            cout << "0. Thoat" << endl;
            cout << "Chon: ";
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1:
                    clrscr();
                    tempAcc.dangky(false);
                    if (!tempAcc.kiemtrataikhoantrung(tempAcc.getUsername())) {
                        loggedInAcc = tempAcc;
                        isLoggedIn = true;
                        account::luuDuLieu();
                        if (loggedInAcc.getLoai() == admin) {
                            account::adminMenu(loggedInAcc);
                            isLoggedIn = false;
                        }
                    }
                    cout << "Enter de tiep tuc";
                    cin.get();
                    break;
                case 2:
                    account::dangnhap(loggedInAcc, isLoggedIn);
                    if (isLoggedIn && loggedInAcc.getLoai() == admin) {
                        account::adminMenu(loggedInAcc);
                        isLoggedIn = false;
                    }
                    break;
                case 0:
                    clrscr();
                    cout << "Thoat chuong trinh...\n";
                    return 0;
                default:
                    cout << "Lua chon khong hop le!\n";
                    cout << "Enter de tiep tuc";
                    cin.get();
            }
            account::luuDuLieu();
        } else {
            account::functionMenu(loggedInAcc, isLoggedIn);
        }
    }
    return 0;
}

