#pragma region INCLUDE
#include <iostream>
#include <fstream>
#include "arrayvector.cpp"
#include <string>
#include <map>
#include <ctime>
using namespace std;
#pragma endregion

#pragma region DEFINE
#define ui unsigned int
#define str string

const double giaCoban = 1.1;
const int maxLength = 25;
const map<str, int> heSoVung = {{"NH", 1}, {"LC", 2}, {"X", 3}, {"RX", 4}};
const str daysOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#pragma endregion

#pragma region OTHER
str getDayOfWeek(const str &date) {
    struct tm timeInfo = {};
    char buffer[80];

    //định dạng ngày nhập vào là "dd/mm/yyyy"
    sscanf(date.c_str(), "%d/%d/%d", &timeInfo.tm_mday, &timeInfo.tm_mon, &timeInfo.tm_year);

    // Điều chỉnh lại tháng (struct tm trong C++ sử dụng tháng từ 0 đến 11)
    timeInfo.tm_mon -= 1; // tháng trong struct tm là từ 0 đến 11
    timeInfo.tm_year -= 1900; // năm bắt đầu từ 1900

    // Chuyển đổi sang thời gian thực
    mktime(&timeInfo);

    return daysOfWeek[timeInfo.tm_wday];
}

// Chuyển đổi thời gian (hh:mm) thành số nguyên để so sánh
ui convertTime(const str &time) {
    str hour;
    for (size_t i = 0; i < time.find('h'); ++i) {
        hour += time[i];
    }
    int h = stoi(hour);
    str minute;
    for (size_t i = time.find('h')+1; i < time.size(); ++i) {
        minute += time[i];
    }
    int m = stoi(minute);
    return h*100 + m;
}
#pragma endregion

#pragma region CHECK
bool laNamNhuan(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return true;
    }
    return false;
}


bool isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12) {
        return false;
    }

    int daysInMonth;

    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            daysInMonth = 31;  // Các tháng có 31 ngày
            break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;  
            break;
        case 2:
            if (laNamNhuan(year)) {
                daysInMonth = 29;  
            } else {
                daysInMonth = 28; 
            }
            break;
        default:
            return false; 
    }

    return (day >= 1 && day <= daysInMonth);
}

bool checkTen(const str &name) {
    if (name.size() > maxLength) return false;
    for (char c : name) {
        if (!isalpha(c) && !isspace(c)) return false;
    }
    return true;
}

// Kiểm tra độ hợp lệ của số điện thoại
bool checkSDT(const str &sdt) {
    if (sdt.size() != 10 || sdt[0] != '0') return false;
    for (char c : sdt) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool checkNgayGoi(const str &date) {
    str d, m, y;
    int dem1 = 0, dem2 = 0;
    for (size_t i = 0; i < date.find('/'); ++i) {
        d += date[i];
        dem1++;
    }
    for (size_t i = dem1+1; i < date.find('/', dem1+1); ++i) {
        m += date[i];
        dem2++;
    }
    for (size_t i = dem1+dem2+2; i < date.size(); ++i) {
        y += date[i];
    }

    if (isValidDate(stoi(d), stoi(m), stoi(y))) return true;
}
#pragma endregion

#pragma region CALL
class CuocGoi {
    str sdt, thoiDiemGoi, ngayGoi, vung;
    int soPhut;
public:
    void nhapCuocGoi(istream &in) {
        getline(in, sdt,';');
        if (!checkSDT(sdt)) {
            cerr <<"Dinh dang so dien thoai khong dung!\n";
        }
        in >> ws >> soPhut;
        in.ignore(1, ';');
        in >> ws;
        getline(in, thoiDiemGoi, ';');
        in >> ws;
        getline(in, ngayGoi, ';');
        if (!checkNgayGoi(ngayGoi)) {
            cerr << "Dinh dang ngay khong dung!\n";
        }
        in >> ws;
        getline(in, vung);
        
    }

    double tinhTien() {
        return giaCoban * soPhut * heSoVung.at(vung);
    }

    bool gioGiamGia() const {
        ui time = convertTime(thoiDiemGoi);
        str day = getDayOfWeek(ngayGoi);
        return (time >= 2300 || time <= 500 || day == "Sunday");
    }

    str getSDT() { return sdt; }
    str getVung() { return vung; }
};
#pragma endregion

#pragma region CUSTOMER
class KhachHang {
    str tenKhachHang, soDienThoai;
    vector<CuocGoi> thongTinCuocGoi;

public:
    void nhapKhachHang(istream &in) {
        getline(in, tenKhachHang, ';');
        in >> ws;
        getline(in, soDienThoai);
        if (!checkSDT(soDienThoai)) {
            cerr << "Dinh dang so dien thoai khong dung!\n";
        }
        if (!checkTen(tenKhachHang)) {
            cerr << "Ten phai la chu cai!\n";
        }
    }

    void themCuocGoi(CuocGoi &cg) {
        if (cg.getSDT() == soDienThoai) {
            thongTinCuocGoi.push_back(cg); 
        }
    }

    void tinhToanVaInKetQua(ofstream &out) {
        double tongTien = 0;
        map<string, int> demCuocGoi;

        for (auto cg : thongTinCuocGoi) {
            double tien = cg.tinhTien();
            if (cg.gioGiamGia()) {
                tien *= 0.7; // Giảm giá 30%
            }
            tongTien += tien;
            demCuocGoi[cg.getVung()]++;
        }

        out << tenKhachHang << "; " << soDienThoai << "; " << tongTien << "; "<< demCuocGoi["NH"] << "; " << demCuocGoi["LC"] << "; " << demCuocGoi["X"] << "; " << demCuocGoi["RX"] << endl;
    }

    str getSDT() { return soDienThoai; }
};
#pragma endregion

#pragma region LIST
class danhSachThongKe {
    vector<KhachHang> danhSachKH;
    vector<CuocGoi> danhSachCG;
public:
    void docDuLieu(const str &fileKH, const str &fileCG) {
        ifstream file1(fileKH), file2(fileCG);
        if (!file1.is_open() || !file2.is_open()) {
            cerr << "khong the mo tep" << endl;
            return;
        }

        str line;

        while (getline(file1, line)) {
            KhachHang kh;
            stringstream ss(line);
            kh.nhapKhachHang(ss);  
            if (!kh.getSDT().empty()) {  // Kiểm tra số điện thoại hợp lệ
                danhSachKH.push_back(kh);
            }
        }

        while (getline(file2, line)) {
            CuocGoi cg;
            stringstream ss(line);
            cg.nhapCuocGoi(ss); 
            danhSachCG.push_back(cg);
        }

        for (auto &kh : danhSachKH) {
            for (auto &cg : danhSachCG) {
                if (kh.getSDT() == cg.getSDT()) {
                    kh.themCuocGoi(cg);  // Gán cuộc gọi cho khách hàng có số điện thoại trùng
                }
            }
        }

        file1.close();
        file2.close();
    }

    void ghiKetQua(const str &fileKetQua) {
        ofstream out(fileKetQua);
        if (!out) {
            cerr << "khong the mo tep kq" << endl;
            return;
        }

        for (auto &kh : danhSachKH) {
            kh.tinhToanVaInKetQua(out);
        }

        out.close();
    }
};
#pragma endregion

#pragma region MAIN
int main() {
    danhSachThongKe ds;
    ds.docDuLieu("khachhang.txt", "cuocgoi.txt");
    ds.ghiKetQua("ketqua.txt");
    return 0;
}
#pragma endregion
